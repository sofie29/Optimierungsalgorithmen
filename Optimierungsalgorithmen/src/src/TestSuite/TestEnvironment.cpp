#include "TestEnvironment.h"
#include "RectangleCreator.h"
#include "BoundingBoxCreator.h"
#include "DataHolder.h"
#include "DataHolderT.h"
#include "SimpleInitialSolution.h"
TestEnvironment::TestEnvironment(int instances, int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLength)
{
    instances_ = instances;
	bestData_ = new DataHolder(rect_amount, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
	data_ = new DataHolder(rect_amount, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
	bestDataT_ = new DataHolderT<DataHolder*>(bestData_);
	dataT_ = new DataHolderT<DataHolder*>(data_);

    initSol_ = new SimpleInitialSolution<DataHolder*>();
    emptyBoxObjective_ = new EmptyBoxObjective<DataHolder*>();
    simpleEmptyBoxObjective_ = new SimpleEmptyBoxObjective<DataHolder*>();
    boxListLengthObjective_ = new BoxListLengthObjective<DataHolder*>();

    ruleBasedNeighbour_ = new RuleBasedNeighbour<DataHolder*>(dataT_, bestDataT_, initSol_);
    geometryBasedNeighbour_ = new GeometryBasedNeighbour<DataHolder*>(dataT_, bestDataT_, initSol_);
    geometryBasedOverlappingNeighbour_ = new GeometryBasedOverlappingNeighbour<DataHolder*>(dataT_, bestDataT_, initSol_);
    emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);

    
    localSearch_ = new LocalSearch<DataHolder*>(ruleBasedNeighbour_, dataT_, bestDataT_, initSol_, simpleEmptyBoxObjective_, boxListLengthObjective_);

    areaSortStrategy_ = new AreaSortingStrategy<DataHolder*>();
    diagonalSortStrategy_ = new DiagonalSortingStrategy<DataHolder*>();
    greedy_ = new Greedy<DataHolder*>(dataT_, bestDataT_, initSol_, areaSortStrategy_, simpleEmptyBoxObjective_, boxListLengthObjective_);



    selectedAlgorithm_ = localSearch_;
}

TestEnvironment::~TestEnvironment() {
    delete ruleBasedNeighbour_;
    ruleBasedNeighbour_ = nullptr;

    delete initSol_;
    initSol_ = nullptr;

    delete localSearch_;
    localSearch_ = nullptr;

    delete greedy_;
    greedy_ = nullptr;

    delete areaSortStrategy_;
    areaSortStrategy_ = nullptr;

    delete diagonalSortStrategy_;
    diagonalSortStrategy_ = nullptr;

    delete geometryBasedNeighbour_;
    geometryBasedNeighbour_ = nullptr;

    delete geometryBasedOverlappingNeighbour_;
    geometryBasedOverlappingNeighbour_ = nullptr;

    delete boxListLengthObjective_;
    boxListLengthObjective_ = nullptr;

    delete simpleEmptyBoxObjective_;
    simpleEmptyBoxObjective_ = nullptr;

    delete dataT_;
    dataT_ = nullptr;

    delete bestDataT_;
    bestDataT_ = nullptr;

    delete data_;
    data_ = nullptr;

    delete bestData_;
    bestData_ = nullptr;

    delete emptyBoxObjective_;
    emptyBoxObjective_ = nullptr;
}

void TestEnvironment::Run(std::string path)
{
    std::ofstream file;
    file.open(path);
    
    for (int i = 0; i < instances_; i++) {
        std::cout << "Instance " << i << "\n";
        for (int k = 0; k < AlgorithmConstants::amountAlgorithms_; ++k) {
            std::cout << "Algo " << k << "\n";
            setAlgorithm(k);
            ProtocollNewLine(file, k);
            for (int j = 0; j < 1000; j++) {
                Metric metric = selectedAlgorithm_->execute(1);
                if (metric.time_ > AlgorithmConstants::maxTime_) break;
                Protocoll(file, metric.score_, metric.time_);
            }
        }
        file << "\n";
        data_->ResetRectanglesForTestEnv();
    }
    file.close();
    std::cout << "done \n";
}

void TestEnvironment::Protocoll(std::ofstream& file, float score, float time)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << score;
    stream << " after " << std::fixed << std::setprecision(2) << time << "ms,";

    std::string s = stream.str();
    const char* chars = s.c_str();
    file << chars;
   
}

void TestEnvironment::ProtocollNewLine(std::ofstream& file, int prefix)
{
    file << "\n" + std::to_string(prefix) + ". " + selectedAlgorithm_->getIdentifier() + ",";
}

void TestEnvironment::setAlgorithm(int index)
{
    
    switch (index) {
    case 0:
        localSearch_->setNeighbourDefinition(ruleBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;      
        break;
    case 1:
      
        localSearch_->setNeighbourDefinition(geometryBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;       
        break;
    case 2:
       
        localSearch_->setNeighbourDefinition(geometryBasedOverlappingNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;       
        break;
    case 3:
        greedy_->setSortStrat(areaSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_; 
        break;
    case 4:
        greedy_->setSortStrat(diagonalSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        break;
    }
    selectedAlgorithm_->reset();
   
}
