#include "basic.h"
#include <unordered_map>
#include <csignal>
#include <time.h>

using namespace std;

const int Midori_perm[16] = {0, 10, 5, 15, 14, 4, 11, 1, 9, 3, 12, 6, 7, 13, 2, 8};
const int code3_B_patterns[4][6]={-1, -1, 0, 0, -1, 1,\
1, 0, 0, 1, 0, -1,\
-1, -1, -1, -1, -1, 2,\
1, 1, 1, 1, 1, -2};


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


// Add 16 binary variables to the model
vector<GRBVar> addVar_16(GRBModel& model, int r, string s){
    vector<GRBVar> x(16);
    for (int i=0; i < 16; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}

// Add 4 binary variables to the model
vector<GRBVar> addVar_4(GRBModel& model, int r, string s){
    vector<GRBVar> x(4);
    for (int i=0; i < 4; i++){
        x[i] = model.addVar(0, 1, 0, GRB_BINARY, s + to_string(r) + "_" + to_string(i));
    }
    model.update();
    return x;
}

// Generate constraints for each round
void genConstraints_of_Round(GRBModel& model, int r, unordered_map<int, vector<GRBVar>> c_map, unordered_map<int, vector<GRBVar>> cr_map){
    //c_map: input coding vars to round r
    //cr_map: output coding vars of round r

    //c_shuff_map: coding vars after shuffle cell
    vector<GRBVar> c_2_shuff(16), c_1_shuff(16), c_0_shuff(16);
    
        //cout<<"debug here"<<endl;
    for (int i = 0; i < 16; i++){           
        c_0_shuff[i] = c_map[0][Midori_perm[i]];
        c_1_shuff[i] = c_map[1][Midori_perm[i]];
        c_2_shuff[i] = c_map[2][Midori_perm[i]];
    } 

    unordered_map<int, vector<GRBVar>> c_shuff_map;
    c_shuff_map[0] = c_0_shuff;
    c_shuff_map[1] = c_1_shuff;
    c_shuff_map[2] = c_2_shuff;
    
    //cout<<"debug here"<<endl;
    //c01,c23 coding variables
    unordered_map<int, vector<GRBVar>> c01_map;
    unordered_map<int, vector<GRBVar>> c23_map;
    for (auto& x: c_map){
        c01_map[x.first] = addVar_4(model, r-1, "c01_"+to_string(x.first)+"_");
        c23_map[x.first] = addVar_4(model, r-1, "c23_"+to_string(x.first)+"_");
    }

    //A01, A23 activity variables
    vector<GRBVar> A01h(4), A01l(4), A23h(4), A23l(4);
    A01h = addVar_4(model, r-1, "A01h_");
    A01l = addVar_4(model, r-1, "A01l_");
    A23h = addVar_4(model, r-1, "A23h_");
    A23l = addVar_4(model, r-1, "A23l_");

    //Ah, Al activity variables
    vector<GRBVar> Ah(16), Al(16);
    Ah = addVar_16(model, r-1, "Ah_");
    Al = addVar_16(model, r-1, "Al_");
    
    
    for (int t = 0; t < 4; t++){// for each column

        //add 0-th and 1-st cell
        vector<GRBVar> var01_list = {c_shuff_map[2][4*t], c_shuff_map[1][4*t],c_shuff_map[0][4*t], c_shuff_map[2][4*t+1], c_shuff_map[1][4*t+1],c_shuff_map[0][4*t+1],c01_map[2][t], c01_map[1][t], c01_map[0][t], A01h[t], A01l[t]};
        
        add_constraints_from_template(model, var01_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);

        //add 2-nd and 3-rd cell
        vector<GRBVar> var23_list = {c_shuff_map[2][4*t+2], c_shuff_map[1][4*t+2], c_shuff_map[0][4*t+2], c_shuff_map[2][4*t+3], c_shuff_map[1][4*t+3], c_shuff_map[0][4*t+3], c23_map[2][t], c23_map[1][t], c23_map[0][t], A23h[t], A23l[t]};

        add_constraints_from_template(model, var23_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);

        //output of MC layer 
        //0: 1 + 23
        vector<GRBVar> var0_list = {c_shuff_map[2][4*t+1], c_shuff_map[1][4*t+1],c_shuff_map[0][4*t+1], c23_map[2][t], c23_map[1][t], c23_map[0][t], cr_map[2][4*t], cr_map[1][4*t] ,cr_map[0][4*t], Ah[4*t], Al[4*t]};

        add_constraints_from_template(model, var0_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);

        //1: 0 + 23
        vector<GRBVar> var1_list = {c_shuff_map[2][4*t], c_shuff_map[1][4*t],c_shuff_map[0][4*t], c23_map[2][t], c23_map[1][t], c23_map[0][t],cr_map[2][4*t + 1], cr_map[1][4*t + 1] ,cr_map[0][4*t + 1], Ah[4*t + 1], Al[4*t + 1]};

        add_constraints_from_template(model, var1_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);

        //2: 01 + 3
        vector<GRBVar> var2_list = {c01_map[2][t], c01_map[1][t], c01_map[0][t], c_shuff_map[2][4 * t + 3], c_shuff_map[1][4 * t + 3], c_shuff_map[0][4 * t + 3], cr_map[2][4*t + 2], cr_map[1][4*t + 2] ,cr_map[0][4*t + 2], Ah[4*t + 2], Al[4*t + 2]};

        add_constraints_from_template(model, var2_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);

        //3: 01 + 2
        vector<GRBVar> var3_list = {c01_map[2][t], c01_map[1][t], c01_map[0][t], c_shuff_map[2][4*t+2], c_shuff_map[1][4*t+2], c_shuff_map[0][4*t+2],  cr_map[2][4*t + 3], cr_map[1][4*t + 3] ,cr_map[0][4*t + 3], Ah[4*t + 3], Al[4*t + 3]};

        add_constraints_from_template(model, var3_list, l_r_o_fAH_fAL_patterns_3bitCode[0], 13, 12);


        //Non-zero output difference indicates non-zero input difference on each column

        //0-th xor 1-st difference，This constraint calculates the difference between two columns and ensures that the input variables in the columns change in different ways
        GRBLinExpr lhs1 = 0;
        for (int i = 0; i <= 3; i++){
            lhs1 += cr_map[2][4*t + i];
            lhs1 += cr_map[1][4*t + i];
        }
        for (int i = 0; i < 4; i++){
            model.addConstr(lhs1 - c_shuff_map[2][4*t + i] - c_shuff_map[1][4*t + i] >= 0);
        }

        //0-th xor 2-nd difference
        lhs1 = 0;
        for (int i = 0; i <= 3; i++){
            lhs1 += cr_map[2][4*t + i];
            lhs1 += cr_map[0][4*t + i];
        }
        for (int i = 0; i < 4; i++){
            model.addConstr(lhs1 - c_shuff_map[2][4*t + i] - c_shuff_map[0][4*t + i] >= 0);
        }

        //0-th xor 3-rd difference
        lhs1 = 0;
        for (int i = 0; i <= 3; i++){
            lhs1 += cr_map[1][4*t + i];
            lhs1 += cr_map[0][4*t + i];
        }
        for (int i = 0; i < 4; i++){
            model.addConstr(lhs1 - c_shuff_map[1][4*t + i] - c_shuff_map[0][4*t + i] >= 0);
        }
    }

}

//Function value_to_pattern_3bitcode maps the input binary values to a more understandable pattern representation for analysis and visualization
string value_to_pattern_3bitcode(unordered_map<int, string> c_value){
    int num = c_value[0].size();
    string res = "";
    for (int j = 0; j < num; j++){
        string s = "";
        //Check the value of s and append the corresponding characters to the result string res according to different situations
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

void parse_sol_Midori(GRBModel& model, int r, int printMode){
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
        print_state_pattern(value_to_pattern_3bitcode(c_value));
    }else{
        cout <<"Illegal printMode!" << endl;
        exit(-1);
    }

    //retrieve intermediate variables
    unordered_map<int, vector<GRBVar>> c01_map;
    unordered_map<int, vector<GRBVar>> c23_map;
    unordered_map<int, string> c01_value;
    unordered_map<int, string> c23_value;
    //retrieve activity variables
    vector<GRBVar> Ah(16), Al(16);
    string Ah_value, Al_value;
    vector<GRBVar> A01h(4), A01l(4), A23h(4), A23l(4);
    string A01h_value, A01l_value, A23h_value, A23l_value;

    
    
    
    try{
    //Go through r cycles, in each cycle, it retrieves and stores the values of the c variable, c01 variable, c23 variable, Ah variable, and Al variable in each state
    for (int j = 1; j <= r; j++){
        for (int i = 0; i < 3; i++){
            c_map[i] = getVarVectorByName(model, "c_"+to_string(i)+"_"+to_string(j)+"_", 16);
            c_value[i] = getVectorVarValue(model, c_map[i]);  

            c01_map[i] = getVarVectorByName(model, "c01_"+to_string(i)+"_"+to_string(j-1)+"_", 4);   
            c01_value[i] = getVectorVarValue(model, c01_map[i]); 
            c23_map[i] = getVarVectorByName(model, "c23_"+to_string(i)+"_"+to_string(j-1)+"_", 4);  
            c23_value[i] = getVectorVarValue(model, c23_map[i]); 
        }
        


        
        Ah = getVarVectorByName(model, "Ah_"+to_string(j-1)+"_", 16);
        Al = getVarVectorByName(model, "Al_"+to_string(j-1)+"_", 16);
        
        Ah_value = getVectorVarValue(model, Ah);
        Al_value = getVectorVarValue(model, Al);

        A01h = getVarVectorByName(model, "A01h_"+to_string(j-1)+"_", 4);
        A01l = getVarVectorByName(model, "A01l_"+to_string(j-1)+"_", 4);
        A23h = getVarVectorByName(model, "A23h_"+to_string(j-1)+"_", 4);
        A23l = getVarVectorByName(model, "A23l_"+to_string(j-1)+"_", 4);

        A01h_value = getVectorVarValue(model, A01h);
        A01l_value = getVectorVarValue(model, A01l);
        A23h_value = getVectorVarValue(model, A23h);
        A23l_value = getVectorVarValue(model, A23l);



        if (printMode == 1){// print a pattern string
            cout<<value_to_pattern_3bitcode(c_value); 
            //cout<<" & ";
            //printPattern_latex(value_to_pattern(e_value));

        }
        else if (printMode == 2){ // print pattern in state
            cout << j << "-th state "<<endl;
            print_state_pattern(value_to_pattern_3bitcode(c_value));
            cout << "c01  = "<< value_to_pattern_3bitcode(c01_value)<<endl;
            cout << "c23  = "<< value_to_pattern_3bitcode(c23_value)<<endl;
            cout << "A01h = " << A01h_value << endl;
            cout << "A01l = " << A01l_value << endl;
            cout << "A23h = " << A23h_value << endl;
            cout << "A23l = " << A23l_value << endl;
            cout << "Ah   = " << Ah_value <<endl;
            cout << "Al   = " << Al_value <<endl;
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
        for (auto& x: c_map){
            cr_map[x.first] = addVar_16(model, i, "c_"+to_string(x.first)+"_");
        }
        
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
    //model.addConstr(Bound >= objbound);
    model.update();

    //---------------set objective function-------------------------------
    GRBLinExpr ActiveVar = 0;
    for (int i = 1; i <= r; i++){
        vector<GRBVar> A01h(4), A01l(4), A23h(4), A23l(4);
        vector<GRBVar> Ah(16), Al(16);

        for (int t = 0; t <= 3; t++){
            A01h[t] = model.getVarByName("A01h_" + to_string(i-1)+"_"+to_string(t));
            A01l[t] = model.getVarByName("A01l_" + to_string(i-1)+"_"+to_string(t));
            A23h[t] = model.getVarByName("A23h_" + to_string(i-1)+"_"+to_string(t));
            A23l[t] = model.getVarByName("A23l_" + to_string(i-1)+"_"+to_string(t));
        }

        for (int s = 0; s <= 15; s++){
            Ah[s] = model.getVarByName("Ah_"+to_string(i-1)+"_"+to_string(s));
            Al[s] = model.getVarByName("Al_"+to_string(i-1)+"_"+to_string(s));
        }

        for (int t = 0; t <= 3; t++){
            ActiveVar += 2 * A01h[t];
            ActiveVar += A01l[t];
            ActiveVar += 2 * A23h[t];
            ActiveVar += A23l[t];
            for (int s = 0; s <= 3; s++){
                ActiveVar += 2 * Ah[4*t + s];
                ActiveVar += Al[4*t + s];
            }
        }
    }

    //Add a constraint condition with ActiveVar less than or equal to Bound -1. This constraint ensures that the sum of active variables is less than or equal to the sum of bound variables minus 1
    model.addConstr(ActiveVar <= Bound -1); 
    model.setObjective(ActiveVar, GRB_MINIMIZE);
    model.update();
    
    cout<< "NumVar "<<model.get(GRB_IntAttr_NumVars)<<endl;
    cout<< "NumConstr "<<model.get(GRB_IntAttr_NumConstrs)<<endl;

    //Print the number of variables and constraints in the model and print the model to "MidoriMin_r. lp"
    model.write("MidoriMin_"+to_string(r)+".lp");


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
    int R = 4;
    //int objbound = 12;
    cout <<"Buiding MILP model for Midori"<<endl;
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

    //model.set("BestObjStop", "6.5");
    mycallback cb = mycallback(30);//terminate if the obj has not changed in 1000seconds
    model.setCallback(&cb);
    model.optimize();
    model.write("MidoriMin_"+to_string(R)+".sol");

    //if (model.get( GRB_IntAttr_Status ) == GRB_USER_OBJ_LIMIT){
        parse_sol_Midori(model, R, 2);

    //}
    
    return 0;
}