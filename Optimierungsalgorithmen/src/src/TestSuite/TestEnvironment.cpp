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
	ruleBasedNeighbour_ = new RuleBasedNeighbour<DataHolder*>(dataT_, bestDataT_, initSol_);
	geometryBasedNeighbour_ = new GeometryBasedNeighbour<DataHolder*>(dataT_, bestDataT_, initSol_);

	localSearch_ = new LocalSearch<DataHolder*>(ruleBasedNeighbour_, dataT_, bestDataT_, initSol_);
    selectedAlgorithm_ = localSearch_;
}

TestEnvironment::~TestEnvironment() {
    delete ruleBasedNeighbour_;
    ruleBasedNeighbour_ = nullptr;

    delete initSol_;
    initSol_ = nullptr;

    delete localSearch_;
    localSearch_ = nullptr;

    delete dataT_;
    dataT_ = nullptr;

    delete bestDataT_;
    bestDataT_ = nullptr;

    delete data_;
    data_ = nullptr;

    delete bestData_;
    bestData_ = nullptr;
}

void TestEnvironment::Run(std::string path)
{
    std::ofstream file;
    file.open(path);
    
    for (int i = 0; i < instances_; i++) {
        ProtocollNewLine(file, i);
        selectedAlgorithm_->reset();
        for (int j = 0; j < 1000; j++) {
            float score = selectedAlgorithm_->execute(1);
            Protocoll(file, score, 0.0);
        }
    }
    file.close();
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
