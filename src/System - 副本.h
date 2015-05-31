#ifndef GUARD_System_h
#define GUARD_System_h

//-------------------------------------------------------------------------------------------
//
// System.h
//
// Author: Struan Robertson
// Date:   11/Feb/2003
//
// This header file contains the declaration of the System class. This class is the route
// of all molecular and reaction data will contain information about any number of System.
// Reaction System inforamtion will be sorted in a vector of reaction maps. Molecular data
// is stored in the molecule manager.
//
//-------------------------------------------------------------------------------------------
#include "plugin.h"
#include "ReactionManager.h"
#include "calcmethod.h"
#include "CollisionOperator.h"

#define MESMER_VERSION "3.0"

namespace mesmer
{
	///Search the library of molecules. If PPMolList is valid copy to the main XML file,
	// replacing an existing molecule of same name. Return a pointer to the copy (or the librymol).
	extern PersistPtr GetFromLibrary(const std::string molName, PersistPtr ppMolList);
	//global variable
	extern std::string libfile;

	class CalcMethod;

	class System 
	{
	public:

		//System();
		System(const std::string& libraryfilename) ;
		~System() ;

		// Initialize the System object.
		bool initialize(void) ;

		// return a pointer to a deepcopy of input System, sys
		//System* deepCopy();

		// Read and parse a data input file.
		bool parse(PersistPtr ppIOPtr) ;

		// Begin calculation.
		void executeCalculation() ;

		// Execute a test calculation and output the thermodynamic data to the .test file.
		void executeTestCalculation(int grainSize, double EAboveTop);

		// Begin single calculation.
		bool calculate(double& chiSquare, vector<double> &residuals, bool writeReport = false) ;

		// Begin single calculation - wrapper function.
		bool calculate(double& chiSquare, bool writeReport = false) {
			vector<double> residuals ;
			return calculate(chiSquare, residuals, writeReport) ;
		} ;

		// Calculate rate coefficients etc. for a specific condition.
		bool calculate(size_t nCond, vector<double> &Quantities, bool writeReport = false) ;

		// Begin single calculation.
		bool calculate(double &Temperature, double &Concentration, Precision precision, map<string, double> &phenRates, double &MaxT);

		// Begin single test calculation.
		bool testCalculate(double& chiSquare, vector<double> &residuals, bool writeReport = false) ;

		// Begin single test calculation - wrapper function.
		bool testCalculate(double& chiSquare, bool writeReport = false) {
			vector<double> residuals ;
			return testCalculate(chiSquare, residuals, writeReport) ;
		} ;

		// Print system configuration
		void configuration(void);

		// Deduce the role of each molecule and add it to the XML file 
		bool assignMolTypes(PersistPtr ppIOPtr) ;

		void WriteMetadata(const std::string& infilename);

		PersistPtr getPersistPtr() { return m_ppIOPtr; }
		PersistPtr getAnalysisPtr() { return m_ppAnalysis; }

		MoleculeManager* getMoleculeManager() { return m_pMoleculeManager; } ;
		ReactionManager* getReactionManager() { return m_pReactionManager; } ;

		// An accessor method to get conditions and related properties for
		// use with plugins classes etc.
		bool getConditions (vector<double> &Temperature, vector<double> &Concentration) ;

		size_t getNumConditions() const { return PandTs.size() ; } ;

		// Mesmer control flags.
		MesmerFlags m_Flags;

		MesmerEnv& getEnv() { return m_Env; } ;

	private:

		void readPTs(PersistPtr);

		bool ReadRange(const std::string&    name,
			std::vector<double>&  vals,
			PersistPtr            ppbase,
			bool                  MustBeThere=true);

		double calcChiSqRateCoefficients(const qdMatrix& mesmerRates, const CandTpair& expData, stringstream &rateCoeffTable, vector<double> &residuals) ;
		double calcChiSqYields(const CandTpair& expData,  stringstream &rateCoeffTable, vector<double> &residuals);
		double calcChiSqEigenvalues(const CandTpair& expData,  stringstream &rateCoeffTable, vector<double> &residuals);

		//Add extra attributes) containing calculated value and timestamp to <me:experimentalRate> (or similar element)
		void AddCalcValToXml(const CandTpair& expData, size_t i, double val) const;

		// Location of the molecule manager.
		MoleculeManager *m_pMoleculeManager;

		// Location of the reaction mananger.
		ReactionManager *m_pReactionManager ;

		// Paired concentration and pressure points.
		std::vector<CandTpair> PandTs;

		// Physical variables. Reference to this are passed to all Molecule and Reaction constructors.
		MesmerEnv m_Env;

		// level in XML file under <mesemer>
		PersistPtr m_ppIOPtr;

		// current <analysis> element
		PersistPtr m_ppAnalysis;

		// The method used for the main calculation
		CalcMethod* m_CalcMethod;

		const char* m_pTitle;
		const char* m_pDescription;

		CollisionOperator m_collisionOperator ;

	} ;

}//namespace
#endif // GUARD_System_h
