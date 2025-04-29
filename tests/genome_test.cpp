//
// Created by tommeh on 28.04.25.
//

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "../include/genome.h"
#include "../include/synapse.h"
#include "../include/node.h"

class SimpleGenomeTests : public ::testing::Test {
protected:
    SimpleGenomeTests() : genome(4, 1) {
        Synapse synapse1(1, 5, 0.5, 1);
        Synapse synapse2(2, 6, 0.2, 2);
        Synapse synapse3(6, 5, 0.1, 3);

        genome.addConnection(synapse1);
        genome.addConnection(synapse2);
        genome.addConnection(synapse3);

        genome.runInferNetwork();
    }

    Genome genome;
};


TEST_F(SimpleGenomeTests, TranslationTest_CorrectNumNodes) {
    auto network = genome.getNodes();
    ASSERT_EQ(network.size(), 6);
}

TEST_F(SimpleGenomeTests, TranslationTest_CorrectlyAddedNode) {
    auto network = genome.getNodes();
    ASSERT_NE(network.find(6), network.end());
}

TEST_F(SimpleGenomeTests, TranslationTest_CorrectlyAddsWeights) {
    const auto network = genome.getNodes();
    const Eigen::VectorXd expectedVector{{0.5, 0.1}};

    ASSERT_EQ(network.at(5)->getWeights(), expectedVector);
}

class HarderGenomeTests : public ::testing::Test {
    protected:
    HarderGenomeTests() : genome(4, 1) {
        Synapse synapse1(1, 6, 0.5, 1);
        Synapse synapse2(2, 6, 0.1, 2);
        Synapse synapse3(6, 9, 0.4, 3);
        Synapse synapse4(2, 9, 0.3, 4);
        Synapse synapse5(3, 7, 1, 5);
        Synapse synapse6(4, 8, -2, 6);
        Synapse synapse7(7, 5, -1, 7);
        Synapse synapse8(8,9,3,8);
        Synapse synapse9(9,5,2,9);

        genome.addConnection(synapse1);
        genome.addConnection(synapse2);
        genome.addConnection(synapse3);
        genome.addConnection(synapse4);
        genome.addConnection(synapse5);
        genome.addConnection(synapse6);
        genome.addConnection(synapse7);
        genome.addConnection(synapse8);
        genome.addConnection(synapse9);


    }
    Genome genome;
};

TEST_F(HarderGenomeTests, TranslationTest_CorrectGeneSize) {
    ASSERT_EQ(genome.getConnections().size(), 9);
}


TEST_F(HarderGenomeTests, TranslationTest_CorrectNetworkSize) {
    genome.runInferNetwork();
    auto network = genome.getNodes();
    ASSERT_EQ(network.size(), 9);
}

TEST_F(HarderGenomeTests, TranslationTest_CorrectlyAddedNodes) {
    genome.runInferNetwork();
    auto network = genome.getNodes();
    ASSERT_NE(network.find(9), network.end());
    ASSERT_NE(network.find(7), network.end());
    ASSERT_NE(network.find(8), network.end());
    ASSERT_NE(network.find(6), network.end());
}

TEST_F(HarderGenomeTests, TranslationTest_CorrectWeights) {
    genome.runInferNetwork();
    const auto network = genome.getNodes();
    const Eigen::VectorXd expectedVector{{0.4, 0.3, 3}};
    ASSERT_EQ(network.at(9)->getWeights(), expectedVector);
}

TEST_F(HarderGenomeTests, TranslationTest_HandlesNegative) {
    genome.runInferNetwork();
    auto network = genome.getNodes();
    ASSERT_EQ(network.at(8)->getWeights()(0), -2);
}

TEST_F(HarderGenomeTests, TranslationTest_HandlesDisabled) {
    genome.getConnectionGenes()[5].changeEnable();
    Synapse newSynapse(4,8,0,10);
    genome.addConnection(newSynapse);
    genome.runInferNetwork();
    auto network = genome.getNodes();
    const Eigen::VectorXd expectedVector{{0}};
    ASSERT_EQ(network.at(8)->getWeights(), expectedVector);
}

// Crossover Tests
class CrossoverTests : public ::testing::Test {
    protected:
    CrossoverTests() : first(3,1), second(3,1) {
        Synapse synapse1(1, 4, 0.5, 1);
        Synapse synapse2(2, 4, 0.1, 2);
        synapse2.changeEnable();
        Synapse synapse3(3,4,0.3,3);
        Synapse synapse4(2,5,0.4,4);
        Synapse synapse5(5,4,-1,5);
        Synapse synapse6(1,5,1,8);
        first.addConnection(synapse1);
        first.addConnection(synapse5);
        first.addConnection(synapse2);
        first.addConnection(synapse6);
        first.addConnection(synapse3);
        first.addConnection(synapse4);


        Synapse otherSynapse1(1, 4, -0.5, 1);
        Synapse otherSynapse2(2, 4, -0.1, 2);
        otherSynapse2.changeEnable();
        Synapse otherSynapse3(3,4,-0.3,3);
        Synapse otherSynapse4(2,5,-0.4,4);
        Synapse otherSynapse5(5,4,-2,5);
        Synapse otherSynapse6(5,6,1.7,6);
        Synapse otherSynapse7(6,4,-1.3,7);
        Synapse otherSynapse8(3,5,-0.2,9);
        Synapse otherSynapse9(1,6,-1.3,10);
        second.addConnection(otherSynapse1);
        second.addConnection(otherSynapse2);
        second.addConnection(otherSynapse3);
        second.addConnection(otherSynapse4);
        second.addConnection(otherSynapse5);
        second.addConnection(otherSynapse6);
        second.addConnection(otherSynapse7);
        second.addConnection(otherSynapse8);
        second.addConnection(otherSynapse9);
    }
    Genome first;
    Genome second;
};

TEST_F(CrossoverTests, CrossoverTest_CorrectNodeSorting) {
    ASSERT_EQ(first.getConnectionGenes()[5].historicalNumber, 8);
}

TEST_F(CrossoverTests, CrossoverTests_CorrectChildLength) {
    auto child = Genome(first, second);
    ASSERT_EQ(child.getConnections().size(), 6);
}

TEST_F(CrossoverTests, CrossoverTests_CorrectMatchingGenes) {
    auto child = Genome(first, second);
    ASSERT_FALSE(child.getConnectionGenes()[1].isEnabled);
}

TEST_F(CrossoverTests, CrossoverTests_CorrectDisjointGenes) {
    auto child = Genome(first, second);
    ASSERT_EQ(child.getConnectionGenes()[5].historicalNumber, 8);
}

TEST_F(CrossoverTests, CrossoverTests_CorrectExcessGenes) {
    auto child = Genome(second, first);
    ASSERT_EQ(child.getConnections().size(), 9);
}


TEST(OutputTests, OutputTests_CorrectInputs) {
    Genome genome(1,1);

    std::vector<double> input;
    input.push_back(5.0);

    genome.runSetInputs(input);

    auto network = genome.getNodes();
    ASSERT_EQ(network[1]->getOutput(), 5.0);
}


TEST(OutputTests, OutputTests_Simple) {
    Genome genome(1,1);
    Synapse synapse1(1, 2, 0.5, 1);

    genome.addConnection(synapse1);
    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(5.0);


    ASSERT_EQ(genome.getOutput(input)[0], 2.5);
}

#include "../include/node_type.h"

TEST(OutputTests, OutputTests_SimpleMultiInput) {
    Genome genome(2,1);
    Synapse synapse1(1, 3, 1.5, 1);
    Synapse synapse2(2, 3, -1, 2);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(1.0);
    input.push_back(1.0);

    ASSERT_EQ(genome.getOutput(input)[0], 0.5);
}

TEST(OutputTests, OutputTests_SimpleMultiOutput) {
    Genome genome(1,2);
    Synapse synapse1(1,2, 1.5,1);
    Synapse synapse2(1,3,5,2);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(1.0);

    std::vector<double> expectedOutput;
    expectedOutput.push_back(1.5);
    expectedOutput.push_back(5.f);


    ASSERT_EQ(genome.getOutput(input), expectedOutput);
}

TEST(OutputTests, OutputTests_MediumOutput) {
    Genome genome(2,1);
    Synapse synapse1(1, 3, -1, 1);
    Synapse synapse2(1, 4, -1, 2);
    Synapse synapse3(2, 4, 2, 3);
    Synapse synapse4(2, 3, -1.4, 4);
    Synapse synapse5(4, 3, 2, 5);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);
    genome.addConnection(synapse5);
    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(0.5);
    input.push_back(1);

    std::vector<double> expectedOutput;
    expectedOutput.push_back(1.1);


    ASSERT_EQ(genome.getOutput(input), expectedOutput);
}

TEST(OutputTests, OutputTests_HandlesDisabled) {
    Genome genome(3,1);
    Synapse synapse1(1, 4, 0.5, 1);
    Synapse synapse2(2, 4, 0.1, 2);
    synapse2.changeEnable();
    Synapse synapse3(3,4,0.3,3);
    Synapse synapse4(2,5,0.4,4);
    Synapse synapse5(5,4,-1,5);
    Synapse synapse6(1,5,1,8);
    genome.addConnection(synapse1);
    genome.addConnection(synapse5);
    genome.addConnection(synapse2);
    genome.addConnection(synapse6);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);

    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(-1);
    input.push_back(5);
    input.push_back(10);

    std::vector<double> expectedOutput;
    expectedOutput.push_back(1.5);

    ASSERT_EQ(genome.getOutput(input), expectedOutput);
}


TEST(OutputTests, OutputTests_FinalBoss) {
    Genome genome(4,1);
    Synapse synapse1(1, 8, -0.2, 1);
    Synapse synapse2(1, 6, 1.4, 2);
    Synapse synapse3(1, 7, 1, 3);
    Synapse synapse4(2, 6, -1, 4);
    Synapse synapse5(2, 9, -0.1, 5);
    Synapse synapse6(4, 10, -3, 6);
    Synapse synapse7(6, 8, -1, 7);
    Synapse synapse8(6, 5, 0.2, 8);
    Synapse synapse9(6, 9, 0.4, 9);
    Synapse synapse10(7, 9, -0.2, 10);
    Synapse synapse11(7, 10, 1, 11);
    Synapse synapse12(8, 5, 100, 12);
    Synapse synapse13(9, 5, -1, 13);
    Synapse synapse14(10, 5, 1, 14);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);
    genome.addConnection(synapse5);
    genome.addConnection(synapse6);
    genome.addConnection(synapse7);
    genome.addConnection(synapse8);
    genome.addConnection(synapse9);
    genome.addConnection(synapse10);
    genome.addConnection(synapse11);
    genome.addConnection(synapse12);
    genome.addConnection(synapse13);
    genome.addConnection(synapse14);
    genome.runInferNetwork();

    std::vector<double> input;
    input.push_back(5);
    input.push_back(2);
    input.push_back(100);
    input.push_back(1);

    std::vector<double> expectedOutput;
    expectedOutput.push_back(2.2);

    ASSERT_EQ(genome.getOutput(input), expectedOutput);
}

TEST(OutputTests, OutputTests_FinalBossMultiQuery) {
    Genome genome(4,1);
    Synapse synapse1(1, 8, -0.2, 1);
    Synapse synapse2(1, 6, 1.4, 2);
    Synapse synapse3(1, 7, 1, 3);
    Synapse synapse4(2, 6, -1, 4);
    Synapse synapse5(2, 9, -0.1, 5);
    Synapse synapse6(4, 10, -3, 6);
    Synapse synapse7(6, 8, -1, 7);
    Synapse synapse8(6, 5, 0.2, 8);
    Synapse synapse9(6, 9, 0.4, 9);
    Synapse synapse10(7, 9, -0.2, 10);
    Synapse synapse11(7, 10, 1, 11);
    Synapse synapse12(8, 5, 100, 12);
    Synapse synapse13(9, 5, -1, 13);
    Synapse synapse14(10, 5, 1, 14);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);
    genome.addConnection(synapse5);
    genome.addConnection(synapse6);
    genome.addConnection(synapse7);
    genome.addConnection(synapse8);
    genome.addConnection(synapse9);
    genome.addConnection(synapse10);
    genome.addConnection(synapse11);
    genome.addConnection(synapse12);
    genome.addConnection(synapse13);
    genome.addConnection(synapse14);
    genome.runInferNetwork();

    std::vector<double> input1;
    input1.push_back(5);
    input1.push_back(2);
    input1.push_back(100);
    input1.push_back(1);
    ;
    std::vector<double> input2;
    input2.push_back(5);
    input2.push_back(10);
    input2.push_back(100);
    input2.push_back(1);

    std::vector<double> expectedOutput;
    expectedOutput.push_back(2.2);

    ASSERT_EQ(genome.getOutput(input1), expectedOutput);
    ASSERT_NE(genome.getOutput(input2), expectedOutput);
}

class CompareTests : public ::testing::Test {
protected:
    CompareTests() : first(3,1), second(3,1) {
        Synapse synapse1(1, 4, 0.5, 1);
        Synapse synapse2(2, 4, 0.1, 2);
        synapse2.changeEnable();
        Synapse synapse3(3,4,0.3,3);
        Synapse synapse4(2,5,0.4,4);
        Synapse synapse5(5,4,-1,5);
        Synapse synapse6(1,5,1,8);
        first.addConnection(synapse1);
        first.addConnection(synapse5);
        first.addConnection(synapse2);
        first.addConnection(synapse6);
        first.addConnection(synapse3);
        first.addConnection(synapse4);


        Synapse otherSynapse1(1, 4, -0.5, 1);
        Synapse otherSynapse2(2, 4, -0.1, 2);
        otherSynapse2.changeEnable();
        Synapse otherSynapse3(3,4,-0.3,3);
        Synapse otherSynapse4(2,5,-0.4,4);
        Synapse otherSynapse5(5,4,-2,5);
        Synapse otherSynapse6(5,6,1.7,6);
        Synapse otherSynapse7(6,4,-1.3,7);
        Synapse otherSynapse8(3,5,-0.2,9);
        Synapse otherSynapse9(1,6,-1.3,10);
        second.addConnection(otherSynapse1);
        second.addConnection(otherSynapse2);
        second.addConnection(otherSynapse3);
        second.addConnection(otherSynapse4);
        second.addConnection(otherSynapse5);
        second.addConnection(otherSynapse6);
        second.addConnection(otherSynapse7);
        second.addConnection(otherSynapse8);
        second.addConnection(otherSynapse9);
    }

    Genome first;
    Genome second;
};

TEST_F(CompareTests, CompareTests_CorrectComparison) {
    auto result = round(first.compareSimilarity(second)*1000);
    ASSERT_EQ(result, 844);
}

TEST_F(CompareTests, CompareTests_Commutativity) {
    auto result1 = first.compareSimilarity(second);
    auto result2 = second.compareSimilarity(first);

    ASSERT_EQ(result1, result2);
}

TEST_F(CompareTests, CompareTests_SimilarToItself) {
    auto result = first.compareSimilarity(first);
    ASSERT_EQ(result, 0);
}

TEST(TopSortTests, TopSortTests_Simple) {
    Genome genome(3,1);
    Synapse synapse1(1, 4, 0.5, 1);
    Synapse synapse2(2, 4, 0.1, 2);
    synapse2.changeEnable();
    Synapse synapse3(3,4,0.3,3);
    Synapse synapse4(2,5,0.4,4);
    Synapse synapse5(5,4,-1,5);
    Synapse synapse6(1,5,1,8);
    genome.addConnection(synapse1);
    genome.addConnection(synapse5);
    genome.addConnection(synapse2);
    genome.addConnection(synapse6);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);

    genome.runInferNetwork();

    std::vector<int> expectedOutput;
    expectedOutput.push_back(2);
    expectedOutput.push_back(1);
    expectedOutput.push_back(3);
    expectedOutput.push_back(5);
    expectedOutput.push_back(4);

    ASSERT_EQ(genome.runTopSort(), expectedOutput);
}

TEST(TopSortTests, TopSortTests_Hard) {
    Genome genome(4,1);
    Synapse synapse1(1, 8, -0.2, 1);
    Synapse synapse2(1, 6, 1.4, 2);
    Synapse synapse3(1, 7, 1, 3);
    Synapse synapse4(2, 6, -1, 4);
    Synapse synapse5(2, 9, -0.1, 5);
    Synapse synapse6(4, 10, -3, 6);
    Synapse synapse7(6, 8, -1, 7);
    Synapse synapse8(6, 5, 0.2, 8);
    Synapse synapse9(6, 9, 0.4, 9);
    Synapse synapse10(7, 9, -0.2, 10);
    Synapse synapse11(7, 10, 1, 11);
    Synapse synapse12(8, 5, 100, 12);
    Synapse synapse13(9, 5, -1, 13);
    Synapse synapse14(10, 5, 1, 14);

    genome.addConnection(synapse1);
    genome.addConnection(synapse2);
    genome.addConnection(synapse3);
    genome.addConnection(synapse4);
    genome.addConnection(synapse5);
    genome.addConnection(synapse6);
    genome.addConnection(synapse7);
    genome.addConnection(synapse8);
    genome.addConnection(synapse9);
    genome.addConnection(synapse10);
    genome.addConnection(synapse11);
    genome.addConnection(synapse12);
    genome.addConnection(synapse13);
    genome.addConnection(synapse14);
    genome.runInferNetwork();

    std::vector<int> expectedOutput;
    expectedOutput.push_back(1);
    expectedOutput.push_back(2);
    expectedOutput.push_back(7);
    expectedOutput.push_back(4);
    expectedOutput.push_back(6);
    expectedOutput.push_back(10);
    expectedOutput.push_back(9);
    expectedOutput.push_back(8);
    expectedOutput.push_back(5);

    ASSERT_EQ(genome.runTopSort(), expectedOutput);
}