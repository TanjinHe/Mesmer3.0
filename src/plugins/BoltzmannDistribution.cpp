//-------------------------------------------------------------------------------------------
//
// BoltzmannDistribution.cpp
//
// Author: Chi-Hsiu Liang
// Date:   _2008_05_15_
//
// Produces Boltzmann distribution
//
//-------------------------------------------------------------------------------------------
#include <vector>
#include <cmath>
#include <string>
#include "../Distribution.h"
#include "../MolecularComponents.h"
#include "../Molecule.h"
#include "../MesmerFlags.h"

namespace mesmer
{
  class BoltzmannDistribution : public DistributionCalculator
  {
  public:

    ///Constructor which registers with the list of DistributionCalculators in the base class
    BoltzmannDistribution(const char* id) :m_id(id){ Register(); }

    virtual ~BoltzmannDistribution() {}
    virtual const char* getID()  { return m_id; }

    virtual bool calculateDistribution(Molecule* m_host, std::vector<double>& dist);

  private:
    const char* m_id;
  };

  //************************************************************
  //Global instance, defining its id (usually the only instance)
  BoltzmannDistribution theBoltzmannDistribution("Boltzmann");
  //************************************************************

  bool BoltzmannDistribution::calculateDistribution(Molecule* m_host,
    std::vector<double>& dist)
  {
    //Get the average grain energies

    vector<double> Ene;
    m_host->getDOS().getGrainEnergies(Ene);

    // Get the rovibrational Density of states

    vector<double> DOS;
    m_host->getDOS().getGrainDensityOfStates(DOS);

    // Get the value of beta

    const double& beta = m_host->getEnv().beta;;

    // Calculate unnormalized Boltzmann dist.
    // Note the extra 10.0 is to prevent underflow, it is removed during normalization.
    dist.clear();
    for (size_t i(0) ; i < DOS.size() ; ++i) {
      dist.push_back(exp(log(DOS[i]) - beta * Ene[i] + 10.0)) ;
    }

    // Calculate the normalization coefficient. Reverse for numerical accuracy.
    double sum(0.0) ;
    for (size_t i(0), j(dist.size()-1) ; i < dist.size() ; ++i,--j) {
      sum += dist[j] ;
    }

    // Normalize distribution. 
    for (size_t i(0) ; i < dist.size() ; ++i) {
      dist[i] /= sum ;
    }

    if (m_host->getFlags().InitialDistEnabled){
      ctest << "\nInitial distribution vector" << endl ;
      for (size_t i(0); i < Ene.size(); i++){
        formatFloat(ctest, dist[i],  6,  15) ;
        ctest << endl ;
      }
    }

    return true;
  }
}//namespace

