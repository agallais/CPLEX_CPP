#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

typedef IloArray<IloNumArray> DataMatrix;


void readTP5datFile(IloModel model, IloNumVarArray x, IloRangeArray constraints , char* nom_de_fichier){

    IloEnv env = model.getEnv();
    char un_char;
    char deuxieme_char;
    //int nb_lu;
    int n;


    ifstream fichier(nom_de_fichier);

    if(fichier){
      //Lecture de n
      fichier >> un_char>> deuxieme_char >> n >> un_char;

      //std::cout << n << std::endl;
      fichier >> un_char >> un_char >> un_char  ;

      //Lecture de q
      DataMatrix q = DataMatrix(env, n);

      for (int i = 0; i < n ; i++){
        fichier >> un_char;
        q[i] = IloNumArray(env);

        for(int j =0; j < n ; ++j){
          int value;
          fichier >> value >> un_char;
          q[i].add(value);
        }
    //std::cout<< q[i] <<std::endl;
        fichier >> un_char;
      }
      fichier >> un_char;

      //Lecture de p
      fichier >>un_char >> un_char >> un_char ;

      IloIntArray a (env);
      for (int i = 0; i < n; i++) {
        int value;
        fichier >> value >> un_char;
        a.add(value);
      }
  //    std::cout<< a<< std::endl;

int b;
fichier >> un_char >> un_char>> un_char >> b;
//std::cout << b << std::endl;

      fichier.close();


      //Now the data is read we go to the real shit

      for (int i = 0; i < n ; ++i){
        x.add(IloBoolVar(env));
      }

IloExpr myExpr(env);
for(int i = 0; i < n; i++){
  for(int j = 0; j < n ; ++j){
    myExpr += q[i][j]* x[i]*x[j];
  }
}

model.add(IloMaximize(env, myExpr ));

constraints.add(IloScalProd(a,x) <= b);

      //constraints.add(IloScalProd(x,d) <= e);

      model.add(constraints);

      //  std::cout <<a << std::endl;

    }
    return ;
}


int main (int argv, char** args) {
  IloEnv env;
  try {
    IloModel model(env);

    IloNumVarArray var(env);
    IloRangeArray con(env);
    readTP5datFile (model, var, con , args[1]);

    IloCplex cplex(model);
    cplex.exportModel("pl.lp");
    cplex.solve();

    env.out() << "Solution status = " << cplex.getStatus() << endl;
    env.out() << "Solution value  = " << cplex.getObjValue() << endl;

    IloNumArray vals(env);
    cplex.getValues(vals, var);
    env.out() << "Values        = " << vals << endl;
    cplex.getSlacks(vals, con);
    env.out() << "Slacks        = " << vals << endl;


  }
  catch (IloException& e) {
    cerr << "Concert exception caught: " << e << endl;
  }
  catch (...) {
    cerr << "Unknown exception caught" << endl;
  }


  env.end();
  return 0;

}
