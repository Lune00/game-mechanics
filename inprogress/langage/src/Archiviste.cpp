#include"Archiviste.hpp"

using namespace std;


Archiviste::Archiviste()
{
  mylibrary_ = "../library/mylibrary.txt";
  importLibrary();
  buildlinks();
}

Archiviste::~Archiviste()
{


}

void Archiviste::importLibrary()
{
  cout<<"Importation de la librairie..."<<endl;
  ifstream mylib(mylibrary_);
  while(true){
    string entree;
    vector<string> tokens;
    getline(mylib,entree);
    if (mylib.eof() ) break;
    tokens = parseEntry(entree,"\t");
    //Check tokens: ils doivent avoir 12 champs definis par mylibrary.txt
    if( tokens.size() < 12) {
      cerr<<"L'entree \""<<tokens[0]<<"\" ne contient pas 12 champs requis."<<endl;
      continue;
    }
    if (tokens.size() == 0 ) continue;
    //From tokens build lists
    addEntry(tokens);
  }
  cout<<"Importation de la librairie...done"<<endl;
}


vector<string> Archiviste::parseEntry(string stringtoparse, const string delimiter ){

  vector<std::string> tokens;
  size_t pos = 0 ;
  string token;
  while( (pos = stringtoparse.find(delimiter)) != std::string::npos)
  {
    token = stringtoparse.substr(0, pos);
    stringtoparse.erase(0, pos + delimiter.length());
    tokens.push_back(token);
  }
  //On rajoute la derniere entree (ce qu'il reste)
  tokens.push_back(stringtoparse);

  //On retire les caracteres vides (dus au formatage en bash)
  //On place tous les caracteres vides a la fin de string et on se place au premier caractere vide
  //puis on supprime jusqu'a la fin (algorithm)
  for(vector<std::string>::iterator it = tokens.begin() ; it != tokens.end() ; it++ ){
    it->erase(std::remove_if(it->begin(), it->end(), ::isspace), it->end());
  }

  return tokens;

}



void Archiviste::addEntry(const vector<string>& entree){
  //L'ordre des champs est defini par la librairie (libconfig.sh)
  string mot = entree[0];
  string phon = entree[2];
  string genre = entree[5];
  string nombre = entree[6];
  int nsyll = stoi(entree[7]);

  //NomC, Adjectif, Verbe ... ?
  string nature = entree[3];

  if( nature == "NOM") {
    NomC a(mot,phon,nsyll,genre[0],nombre[0]);
    nomsC_.push_back(a);
  }
  else if ( nature == "ADJ"){
    Adjectif a(mot,phon,nsyll,genre[0],nombre[0]);
    adjectifs_.push_back(a);
  }
  else if (nature == "VER"){
    Verbe a(mot,phon,nsyll);
    verbes_.push_back(a);
  }
  else{
    cerr<<"L'Archiviste ne connait pas la catégorie "<<nature<<" du mot "<<mot<<". Entree écartée."<<endl;
    return ;
  }

}

void Archiviste::afficher() const{
  cout<<"Nombre d'adjectifs:"<<getNadjectifs()<<endl;
  cout<<"Nombre de noms communs:"<<getNnomsC()<<endl;
  cout<<"Nombre de verbes:"<<getNverbes()<<endl;
}

//Une fois les entrees chargees, on cree des liens entre
//les mots et leurs synonymes et related. Seuls les
//references ayant aussi une entree sont comptés.
//Pour cela on fait en deux temps. D'abord on a chargé
//Les entrées, maintenant on peut facilement checker
//si le synonymes/related sont aussi en tant qu'entrée.
void Archiviste::buildlinks(){

  cout<<"Création des liens entres mots..."<<endl;
  ifstream mylib(mylibrary_);

  while(true){
    string entree;
    vector<string> tokens;
    getline(mylib,entree);
    if (mylib.eof() ) break;
    tokens = parseEntry(entree,"\t");
    //Check tokens: ils doivent avoir 12 champs definis par mylibrary.txt
    if( tokens.size() < 12) {
      cerr<<"L'entree \""<<tokens[0]<<"\" ne contient pas 12 champs requis."<<endl;
      continue;
    }
    if(tokens.size() == 0 ) continue;
    //From tokens build links
    link(tokens);
  }

  cout<<"Création des liens entre mots...done"<<endl;
}


//Cree les liens entre les entrees de la bibliotheque
//Pour chaque entree on regarde ses synonymes/associes
//puis si ils sont trouves comme entree également
//on stocke pour l'entree un pointeur sur eux
void Archiviste::link(vector<string>& tokens){

  //Mot auquel on va rattacher syn et associes
  string mot = tokens[0];
  string nature = tokens[3];
  string delimiter = ";";
  //On recupere les synonymes indiques
  vector<string> syn = parseEntry(tokens[9],delimiter);
  vector<string> associes = parseEntry(tokens[10],delimiter);

  //On recupere le mot que l'on va linker au synonymes/associes
  Mot * m = findword(mot,nature);

  if( m == NULL){
    cerr<<"Archiviste::link : Le mot "<<mot<<" n'a pas été chargé dans la librairie"<<endl;
    return ;
  }

  //On parcourt les synonymes du mot m:
  //On le cherche dans chaque liste:

  //En fait on peut pas connaitre la classe grammaticale
  //du mot synonnymes ou associe a partir de la librairie
  //Mais bon c'est un petit side effect pas forcement
  //indesirable. 
  //Pour le moment on prend tous les mots si on les
  //trouve, peu importe leur nature_

  //Repetition de ce qu'il y a au dessus...
  //Pour le moment on doit prendre tous les synonymes car on ne sait
  //pas si le synonymes est un nom, un adj...
  //ex: Si l'entree a "droit" comme synonyme ca peut etre 'droit l'adjectif' ou 'droit le nom'
  //on les ajoute tous les deux

  //Utiliser find demanderait de creer un Mot
  //or Mot est une classe abstraite...
  //A voir si ca pose des pbs plus tard...

  //ADJ
  for(vector<Mot>::iterator ita = adjectifs_.begin();ita!=adjectifs_.end();ita++){
    //Synonymes:
    for(vector<string>::iterator it = syn.begin();it!=syn.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	//On cree un pointeur qui pointe sur ita
	//On l'ajoute dans les synonymes de m
	Mot * linked = &(*ita);
	m->linksynonymes(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }

    //Associes:
    for(vector<string>::iterator it = associes.begin();it!=associes.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	//On cree un pointeur qui pointe sur ita
	//On l'ajoute dans les synonymes de m
	Mot * linked = &(*ita);
	m->linkassocies(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }
  }

  //VER
  for(vector<Mot>::iterator ita = verbes_.begin();ita!=verbes_.end();ita++){
    //Synonymes:
    for(vector<string>::iterator it = syn.begin();it!=syn.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	Mot * linked = &(*ita);
	m->linksynonymes(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }
    //Associes:
    for(vector<string>::iterator it = associes.begin();it!=associes.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	//On cree un pointeur qui pointe sur ita
	//On l'ajoute dans les synonymes de m
	Mot * linked = &(*ita);
	m->linkassocies(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }
  }

  //NOMC
  for(vector<Mot>::iterator ita = nomsC_.begin();ita!=nomsC_.end();ita++){
    //Synonymes:
    for(vector<string>::iterator it = syn.begin();it!=syn.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	Mot * linked = &(*ita);
	m->linksynonymes(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }
    //Associes:
    for(vector<string>::iterator it = associes.begin();it!=associes.end();it++){
      if( *it == ita->getmot() ) {
	//On l'associe:
	//On cree un pointeur qui pointe sur ita
	//On l'ajoute dans les synonymes de m
	Mot * linked = &(*ita);
	m->linkassocies(linked);
	break ; // il ne peut y avoir 2 mots identiques
	// de meme nature_
      }
    }
  }
}

//Renvoie un pointeur sur un mot correspondant au couple (unique) "mot+nature" dans la bibliotheque
//pointeur utilise pour modifier le mot
Mot * Archiviste::findword(const string& mot, const string& nature) {

  if( nature == "ADJ" ){
    return findADJ(mot);
  }

  else if( nature == "NOM" ){
    return findNOMC(mot);
  }

  else if ( nature == "VER" ){
    return findVER(mot);
  }
  else{
    cerr<<"Mot "<<mot<<" nature: "<<nature<<" introuvable."<<endl;
    return NULL;
  }
}

//Search and return a pointer to an adjective (can be modified) present in the loaded library
Mot * Archiviste::findADJ(const string& mot){
  for(vector<Mot>::iterator ita = adjectifs_.begin();ita!=adjectifs_.end();ita++){
    if(mot == ita->getmot()){
      Mot * m = &(*ita);
      return m;
      break;
    }
  }
  return NULL;
}

//Search and return a pointer to a verb (can be modified) present in the loaded library
Mot * Archiviste::findVER(const string& mot){
  for(vector<Mot>::iterator ita = verbes_.begin();ita!=verbes_.end();ita++){
    if(mot == ita->getmot()){
      Mot * m = &(*ita);
      return m;
      break;
    }
  }
  return NULL;
}

//Search and return a pointer to a nomC (can be modified) present in the loaded library
Mot * Archiviste::findNOMC(const string& mot){
  for(vector<Mot>::iterator ita = nomsC_.begin();ita!=nomsC_.end();ita++){
    if(mot == ita->getmot()){
      Mot * m = &(*ita);
      return m;
      break;
    }
  }
  return NULL;
}
