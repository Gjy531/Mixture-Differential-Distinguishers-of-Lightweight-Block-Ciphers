//g++ Twine.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Twine.out
//nohup ./Twine.out

#include "basic.h"
#include <unordered_map>
#include <csignal>
#include <time.h>

using namespace std;

const int Twine_perm[16] = {5,0,1,4,7,12,3,8,13,6,9,2,15,10,11,14};
const int Twine_n_perm[16] = {1,2,11,6,3,0,9,4,7,10,13,14,5,8,15,12};

const int l_r_o_fAH_fAL_patterns_3bitCode[13][12] = 
{-1, -3, -1, -1, -3, -1, -2, -3, -2, -2, -3, 9,\
3, 3, 2, 3, 3, 2, -1, -1, -2, -6, -4, 0,\
0, 0, 0, -1, -2, -1, 1, 2, 1, 1, 2, 0,\
-3, -4, -3, 0, 1, 0, 1, 0, 1, 3, -1, 4,\
0, -1, -1, -2, 0, -3, 0, -1, 4, 5, 2, 2,\
-1, 0, 0, -1, 0, 0, 2, -1, -1, 2, 1, 1,\
-1, -2, -1, 0, 1, 0, 1, 2, 1, 1, 1, 0,\
0, 1, 1, 0, 1, 1, -2, -1, -1, -2, -2, 2,\
0, 0, -1, 0, 0, -1, -1, -1, 2, 2, 1, 1,\
0, 1, -2, -3, -4, -1, 3, 2, 1, 3, 1, 2,\
2, 1, 2, 2, 0, 1, -1, 0, -1, -3, -1, 0,\
-2, -1, 1, 1, 0, 0, 2, 1, 1, 0, 1, 0,\
1, 1, 0, 1, 1, 0, -1, -1, -2, -2, -2, 2};

int code3_B_patterns[4][6]={-1, -1, 0, 0, -1, 1,\
1, 0, 0, 1, 0, -1,\
-1, -1, -1, -1, -1, 2,\
1, 1, 1, 1, 1, -2};
set<string> code3_B_points {"00011","01010","10010","00110","10100"};


//Add variables, add 16 variables
vector<GRBVar> addVar_16(GRBModel& model, int r, string s){
    vector<GRBVar> x(16);
    for (int i=0; i < 16; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}

//Add variables, add 8 variables
vector<GRBVar> addVar_8(GRBModel& model, int r, string s){
    vector<GRBVar> x(8);
    for (int i=0; i < 8; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}


void genConstraints_of_Round(GRBModel& model, int r, unordered_map<int, vector<GRBVar>> c_map, unordered_map<int, vector<GRBVar>> cr_map){
    //c_map: input coding vars to round r
    //cr_map: output coding vars of round r

    //c_shuff_map: coding vars after the column confusion(coding vars before the permutation)

    vector<GRBVar> c_2_shuff(16), c_1_shuff(16), c_0_shuff(16);
    
    for (int i = 0; i < 16; i++){           
        c_0_shuff[i] = cr_map[0][Twine_perm[i]];
        c_1_shuff[i] = cr_map[1][Twine_perm[i]];
        c_2_shuff[i] = cr_map[2][Twine_perm[i]];
    } 
    unordered_map<int, vector<GRBVar>> c_shuff_map;
    c_shuff_map[0] = c_0_shuff;
    c_shuff_map[1] = c_1_shuff;
    c_shuff_map[2] = c_2_shuff;

    //activity variables
    vector<GRBVar> Ah(8), Al(8);
    Ah = addVar_8(model, r-1, "Ah_");
    Al = addVar_8(model, r-1, "Al_");

    for (int t = 0; t < 8; t++){
        
        vector<GRBVar> var_list = {c_map[2][2*t], c_map[1][2*t], c_map[0][2*t], c_map[2][2*t+1], c_map[1][2*t+1], c_map[0][2*t+1], c_shuff_map[2][2*t+1], c_shuff_map[1][2*t+1], c_shuff_map[0][2*t+1], Ah[t], Al[t]};

        add_constraints_from_template(model, var_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);
    }
    

}

string value_to_pattern_3bitcode(unordered_map<int, string> c_value){
    int num = c_value[0].size();
    string res = "";
    for (int j = 0; j < num; j++){
        string s = "";
        for (int i = 2; i >= 0; i--){ //c2 c1 c0
            s += c_value[i][j];
        }
        if (s=="000"){
            res += "-";
        }
        else if (s == "101"){
            res += "*";
        }
        else if (s == "010"){
            res += "c";
        }
        else if (s == "100"){
            res += "x";
        }
        else if (s == "001"){
            res += "s";
        }
        else{
            res += "o";
            cout<<"illegal pattern"<<endl;
        }
    }
    return res;
}

void parse_sol_Twine(GRBModel& model, int r, int printMode){
    //Retrieve input state
    unordered_map<int, vector<GRBVar>> c_map;
    unordered_map<int, string> c_value;
    
    for (int i = 0; i < 3; i++){
        c_map[i] = getVarVectorByName(model, "c_"+to_string(i)+"_0_", 16);
        c_value[i] = getVectorVarValue(model, c_map[i]);
    }

    if (printMode == 1){// print a pattern string
        cout<<value_to_pattern_3bitcode(c_value); 
        //printPattern_latex(value_to_pattern(e_value));
    }
    else if (printMode == 2){ // print pattern in state
        cout << "0-th state "<<endl;
        value_to_pattern_3bitcode(c_value);
    }else{
        cout <<"Illegal printMode!" << endl;
        exit(-1);
    }

    //retrieve activity variables
    
    vector<GRBVar> Ah(8), Al(8);
    string  Ah_value, Al_value;

    
    
    
    try{
    for (int j = 1; j <= r; j++){
        for (int i = 0; i < 3; i++){
            //c_map[i] = getVarVectorByName(model, "c_"+to_string(i)+"_"+to_string(j)+"_", 16);
            for (int c = 0; c < 8; c++){//
                //c_map[i][Twine_perm[2*c]] = model.getVarByName("c_"+to_string(i)+"_"+to_string(j-1)+"_"+to_string(2*c));
                c_map[i][2*c+1] = model.getVarByName("c_"+to_string(i)+"_"+to_string(j-1)+"_"+to_string(Twine_n_perm[2*c+1]));
                c_map[i][2*c] = model.getVarByName("c_"+to_string(i)+"_"+to_string(j)+"_"+to_string(2*c));
            }

            c_value[i] = getVectorVarValue(model, c_map[i]);  
             
        }
        


        Ah = getVarVectorByName(model, "Ah_"+to_string(j-1)+"_", 8);
        Al = getVarVectorByName(model, "Al_"+to_string(j-1)+"_", 8);
       

        Ah_value = getVectorVarValue(model, Ah);
        Al_value = getVectorVarValue(model, Al);


        
        if (printMode == 1){// print a pattern string
            cout<<value_to_pattern_3bitcode(c_value); 
            //cout<<" & ";
            //printPattern_latex(value_to_pattern(e_value));

        }
        else if (printMode == 2){ // print pattern in state
            cout << j << "-th state "<<endl;
            value_to_pattern_3bitcode(c_value);
            
            cout << "Ah = " << Ah_value << endl;
            cout << "Al = " << Al_value << endl;
        }
        
    }
    }
    catch(GRBException e){
        cout << e.getMessage()<< endl;
    }
    //cout<<"\\\\"<<endl;
    //cout<<"\\hline"<<endl;
    
}

GRBModel genModel(GRBEnv env, int r){
    

    GRBModel model = GRBModel(env);
    
    //input coding variables
    vector<GRBVar> c_2(16), c_1(16), c_0(16);
    c_2 = addVar_16(model, 0, "c_2_");
    c_1 = addVar_16(model, 0, "c_1_");
    c_0 = addVar_16(model, 0, "c_0_");
    unordered_map<int, vector<GRBVar>> c_map;
    c_map[0] = c_0; 
    c_map[1] = c_1;
    c_map[2] = c_2;

    
    //non-trivial input
    GRBLinExpr lhs21, lhs20, lhs10;
    lhs21 = 0; lhs20 = 0; lhs10 = 0;
    for (int i = 0; i <= 15; i++){
        lhs21 += c_map[2][i];
        lhs21 += c_map[1][i];
        lhs20 += c_map[2][i];
        lhs20 += c_map[0][i];
        lhs10 += c_map[1][i];
        lhs10 += c_map[0][i];
    }
    model.addConstr(lhs21 >= 1);
    model.addConstr(lhs20 >= 1);
    model.addConstr(lhs10 >= 1);
    model.update();

    //--------Generate constraints for each round---------------
    for (int i = 1; i <= r; i++){
        unordered_map<int, vector<GRBVar>> cr_map;
        vector<GRBVar> cr_2(16), cr_1(16), cr_0(16);
        for (int k = 0; k< 8; k++){ 

            cr_2[2*k+1] = c_map[2][Twine_n_perm[2*k+1]];
            cr_1[2*k+1] = c_map[1][Twine_n_perm[2*k+1]];
            cr_0[2*k+1] = c_map[0][Twine_n_perm[2*k+1]];

            cr_2[2*k] = model.addVar(0, 1, 0, GRB_BINARY, "c_2_" + to_string(i) + "_" + to_string(2*k));
            cr_1[2*k] = model.addVar(0, 1, 0, GRB_BINARY, "c_1_" + to_string(i) + "_" + to_string(2*k));
            cr_0[2*k] = model.addVar(0, 1, 0, GRB_BINARY, "c_0_" + to_string(i) + "_" + to_string(2*k));
        }
        
                 
        cr_map[0] = cr_0; 
        cr_map[1] = cr_1;
        cr_map[2] = cr_2;
        

        
        genConstraints_of_Round(model, i, c_map, cr_map);
        c_map = cr_map;
        
    }

    //Bound vars
    vector<GRBVar> Bh(16), Bl(16);
    Bh = addVar_16(model, r, "Bh_");
    Bl = addVar_16(model, r, "Bl_");
    for (int i = 0; i <= 15; i++){
        vector<GRBVar> var_list = {c_map[2][i], c_map[1][i], c_map[0][i], Bh[i], Bl[i]};
        add_constraints_from_template(model, var_list, code3_B_patterns[0], 4, 6);
    }
    
    GRBLinExpr Bound = 0;
    for (int i = 0; i <= 15; i++){
        Bound += 2 * Bh[i];
        Bound += Bl[i];
    }
    //model.addConstr(Bound >= objbound); //Make the boundary under random permutation greater than or equal to a certain value
    model.update();

    //---------------set objective function-------------------------------
    GRBLinExpr ActiveVar = 0;
    for (int i = 1; i <= r; i++){
        vector<GRBVar> Ah(8), Al(8);

        for (int t = 0; t <= 7; t++){
            Ah[t] = model.getVarByName("Ah_" + to_string(i-1)+"_"+to_string(t));
            Al[t] = model.getVarByName("Al_" + to_string(i-1)+"_"+to_string(t));
        }



        for (int t = 0; t <= 7; t++){
            ActiveVar += 2 * Ah[t];
            ActiveVar += Al[t];
        }
    }
    model.addConstr(ActiveVar <= Bound -1); //Make the probability on cryptographic algorithms strictly greater than random permutation
    model.setObjective(ActiveVar, GRB_MINIMIZE);
    model.update();
    
    cout<< "NumVar "<<model.get(GRB_IntAttr_NumVars)<<endl;
    cout<< "NumConstr "<<model.get(GRB_IntAttr_NumConstrs)<<endl;

    model.write("TwineMin_"+to_string(r)+".lp");


    return model;
    
}

class mycallback: public GRBCallback{
    public:
        double cur_obj;
        unsigned int cur_time;
        unsigned int time_bound;
        int solcnt;
        mycallback(unsigned int sec){
            cur_obj = - GRB_INFINITY;
            cur_time = time(NULL);
            time_bound = sec;
            solcnt = 0;
        }
    protected:
        void callback (){
            try{
                if (where == GRB_CB_MIPNODE){
                    //get model bojective
                    double objbst = getDoubleInfo(GRB_CB_MIPNODE_OBJBST);
                    solcnt = getIntInfo(GRB_CB_MIPNODE_SOLCNT);

                    // has objective changed? If changed,
                    if (abs(cur_obj - objbst)> 1e-8){
                        cur_obj = objbst;
                        cur_time = time(NULL);
                    }
                }
                //terminate if objective has not changed in secs
                if ((time(NULL) - cur_time > time_bound) && solcnt){
                    cout << cur_obj <<endl;
                    cout << "Stop early - objective has not changed for "<< time_bound <<" seconds" << endl;
                    abort();
                }
            } catch (...) {
                cout << "Error during callback" << endl;
            }

        }
};


int main(){
    cout <<"Building MILP model for Twine"<<endl;
    int R =14 ;
    //int objbound = 12;
    GRBEnv env = GRBEnv();
    //if (objbound == INFINITY){
        env.set(GRB_IntParam_LogToConsole, 1);
        env.set(GRB_IntParam_PoolSearchMode, 0);  //find an optimal solution
    //}else{
    //    env.set(GRB_IntParam_LogToConsole, 0);
    //    env.set(GRB_IntParam_PoolSearchMode, 2);  //PoolSearchMode
    //}

    env.set(GRB_IntParam_Threads, threads );
    env.set(GRB_IntParam_PoolSolutions, MAX); 
    
    GRBModel model = genModel(env, R);

    //model.set("BestObjStop", "7");
    mycallback cb = mycallback(1000);//terminate if the obj has not changed in 1000seconds
    model.setCallback(&cb);
    model.optimize();
    model.write("TwineMin_"+to_string(R)+".sol");

    //if (model.get( GRB_IntAttr_Status ) == GRB_USER_OBJ_LIMIT){
        parse_sol_Twine(model, R, 2);

    //}
    
    return 0;
}