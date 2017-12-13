#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

//static void

/*setdata (, int n , double b, double e)
{
IloEnv env = model.getEnv();

for (int i = 0; i < n ; ++i){
x.add(IloBoolVar(env, ("x%s",n) ));
}

model.add(IloMaximize(env, IloScalProd( , x) ));

c.add( IloScalProd(a,x) == b);
c.add(IloScalProd(d,x) <= e);
c.add( 4*x[0]+ 2*x[1] + 6*x[2] + 1*x[3] + 4*x[4] <= 68);

model.add(c);
}*/

void readTP2datFile(IloModel model, IloNumVarArray x, IloRangeArray constraints , char* nom_de_fichier){

  IloEnv env = model.getEnv();
  char un_char;
  char deuxieme_char;
  int nb_lu;
  int n= 1 ;
  ifstream fichier(nom_de_fichier);
  if(fichier){
    fichier >> un_char>> deuxieme_char >> n >> un_char;

    //Lecture de c
    IloIntArray c(env);
    fichier >> un_char >> un_char >>un_char;
    for (int i = 0; i < n ; ++i){
      int value;
      fichier >> value >> un_char;
      c.add(value);
    }
    fichier >> un_char;
    //Lecture de a
    IloIntArray a(env);
    fichier >> un_char >> un_char >>un_char;
    for (int i = 0; i < n ; ++i){
      int value;
      fichier >> value >> un_char;
      a.add(value);
    }
    fichier >> un_char;
    //Lecture de b
    int b  ;
    fichier >> un_char >> un_char >>b;
    fichier >> un_char;
    //Lecture de d
    IloIntArray d (env);
    fichier >> un_char >> un_char >>un_char;
    for (int i = 0; i < n ; ++i){
      int value;
      fichier >> value >> un_char;
      d.add(value);
    }
    fichier >> un_char;
    //Lecture de e
    int e  ;
    fichier >> un_char >> un_char >>e;
    fichier.close();

    //Now the data is read we go to the real shit

    for (int i = 0; i < n ; ++i){
      x.add(IloBoolVar(env ));
    }

    model.add(IloMaximize(env, IloScalProd(c , x) ));

    constraints.add(IloScalProd(x,a) == b);
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
    readTP2datFile (model, var, con , args[1]);

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
