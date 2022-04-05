#include "TestEnvironment.h"
#include "RectangleCreator.h"
#include "BoundingBoxCreator.h"
#include "DataHolder.h"
#include "DataHolderT.h"
#include "SimpleInitialSolution.h"
TestEnvironment::TestEnvironment(int boxLength)
{
    min_rect_width_vector_ = { 10, 20, 30, 10, 50 };
    max_rect_width_vector_ = { 13, 20, 50, 50, 100 };
    min_rect_height_vector_ = { 10, 20, 10, 10, 50 };
    max_rect_height_vector_ = { 17, 20, 13, 50, 100 };

    instances_ = min_rect_height_vector_.size();
    int rect_amount = 1000;
    int min_rect_width = min_rect_width_vector_[0];
    int min_rect_height = min_rect_height_vector_[0];
    int max_rect_width = max_rect_width_vector_[0];
    int max_rect_height = max_rect_height_vector_[0];
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

TestEnvironment::TestEnvironment(int instances, int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLength)
{
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

    min_rect_width_vector_ = {10, 20, 30, 10, 50};
    max_rect_width_vector_ = {13, 20, 50, 50, 100};
    min_rect_height_vector_ = {10, 20, 10, 10, 50};
    max_rect_height_vector_ = {17, 20, 13, 50, 100};

    instances_ = min_rect_height_vector_.size();
}

TestEnvironment::~TestEnvironment() {

    entryList_.clear();
    entryList_.shrink_to_fit();


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
    
    Metric metric = { 0.0, 0.0 };
    for (int i = 0; i < instances_; i++) {
        std::cout << "Instance " << i << "\n";
        data_->ResetRectanglesForTestEnv(1000, min_rect_width_vector_[i], max_rect_width_vector_[i], min_rect_height_vector_[i], max_rect_height_vector_[i]);
        for (int k = 0; k < AlgorithmConstants::amountAlgorithms_; ++k) {
            std::cout << "Algo " << k << "\n";
            setAlgorithm(k);
            ProtocollNewLine(file, k);
            file << "[" + std::to_string(min_rect_width_vector_[i]) + "-" + std::to_string(max_rect_width_vector_[i]) + "] [" + std::to_string(min_rect_height_vector_[i]) + "-" + std::to_string(max_rect_height_vector_[i]) + "]" + ",";
            for (int j = 0; j < AlgorithmConstants::maxIterations; j++) {
                if (((k == 3 || k == 4) && j == 0) || k < 3) {
                    metric = selectedAlgorithm_->execute(1);
                }
              
                if (j % 2000 == 0) std::cout << j << "\n";
                //if (metric.time > AlgorithmConstants::maxTime) break;
                Protocoll(file, metric.score_, metric.time_);
            }
            int size = static_cast<int>(entryList_.size());
            std::cout <<size << "\n";
            if (entryList_.size() < 1000) {
                for (int l = 0; l < entryList_.size(); ++l) {
                    const char* chars = entryList_[l].c_str();
                    file << chars;
                }
               
            }
            else {
                for (int m = 0; m < 897; ++m) {
                    const char* chars = entryList_[m].c_str();
                    file << chars;
                }
                file << ",";

                for (int n = entryList_.size() - 97; n < entryList_.size(); ++n) {
                    const char* chars = entryList_[n].c_str();
                    file << chars;
                }
            }

            entryList_.clear();

        }
        file << "\n";
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
    entryList_.emplace_back(s);
    //const char* chars = s.c_str();

    //file << chars;
   
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
