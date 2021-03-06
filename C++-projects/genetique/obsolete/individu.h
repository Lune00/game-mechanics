#ifndef INDIVIDU_H
#define INDIVIDU_H

#include<vector>
#include<string>

#include"rng.h"
#include"chromosome.h"

class Gene;

//class Cariotype{
//
//  enum Sexe { masculin , feminin, neutre };
//
//  //Peut etre un ou deux
//  std::vector<Cariotype> Parents_;
//  //Peut prendre n'importe quelle valeur a priori
//  std::vector<Cariotype> Enfants_;
//
//  Sexe sexe_;
//  std::vector<Chromosome> Chromosomes_ ; // plutot des struct de paire de chromosomes (diploides toujours)
//  std::vector<Trait> Traits_;
//
//};

//class Trait{
//
//  std::string nom_;
//  double valeur_;
//  std::vector<Gene> genes_codants_;
//
//};


class Individu{

  struct Parents {
    Individu * pere ;
    Individu * mere ;
  };

  enum Sexe { masculin , feminin };

  public:
  
  Individu(int nbre_genes, const std::vector<Gene>& population);
  Individu(int sexe, int nbre_genes, const std::vector<Gene>& population);
  //Individu(Parents&)

  //acces au donnees externes:
  std::string recevoir_nom_au_hasard(Sexe);
  std::string recevoir_prenom_au_hasard(Sexe);

  Chromosome lire_chromosome_A() const { return chromosome_A_ ; } 
  Chromosome lire_chromosome_B() const { return chromosome_B_ ; } 

  void initialisation_phenotype() ;

  //Affichage:
  void affiche_etat_civil() const ;
  void affiche_genome() const ;
  void affiche_genome_schema() const;
  void affiche_phenotype() const ;
  void affiche_identite_complete() const { affiche_etat_civil() ; affiche_genome_schema() ; affiche_phenotype();}

  private:
  //tmp (a mettre en static)
  RandomGenerator rng_;

  std::string nom_ ;
  std::string prenom_;
  Sexe sexe_;

  Parents parents_;
  //Genome: pour l'instant diploide
  Chromosome chromosome_A_ ;
  Chromosome chromosome_B_ ;

  //Traits phénotypes pours lesquel codent les genes, la paire de chromosomes: O C E A N (calculé par l'expression des genes != potentiel genetique)
  double Ouverture_ ;         // -> expression des genes_[0]
  double Conscienciosité_ ;   // -> expression des genes_[1]
  double Extraversion_ ;      // -> expression des genes_[2]
  double Agreabilité_;        // -> expression des genes_[3]    
  double Neuroticisme_ ;      // -> expression des genes_[4]
};

#endif // INDIVIDU_H
