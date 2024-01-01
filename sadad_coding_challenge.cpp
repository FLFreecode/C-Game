
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

class Box
{
public:
    explicit Box(double initial_weight) : weight_(initial_weight) {}

    static std::unique_ptr<Box> makeGreenBox(double initial_weight);
    static std::unique_ptr<Box> makeBlueBox(double initial_weight);

    bool operator<(const Box &rhs) const { return weight_ < rhs.weight_; }

    double getWeight() const { return weight_; }

    virtual unsigned int ​learnWeight(unsigned int w) = 0; // Abstract implementation for performance
    virtual unsigned int getScore() const = 0; // Abstract implementation for performance

protected:
    double weight_;
};

class GreenBox : public Box
{
public:
    explicit GreenBox(double initial_weight) : Box(initial_weight) {}

    unsigned int ​learnWeight(unsigned int w) override
    {
        weights_.push_back(w);
        if (weights_.size() > 3)
        {
            weights_.pop_front();
        }
        return w;
    }

    unsigned int getScore() const override
    {
        double mean = 0.0;
        if (weights_.empty())
        {
            return 0;
        }
        else if (weights_.size() == 1)
        {
            mean = weights_.front();
        }
        else if (weights_.size() == 2)
        {
            mean = (weights_.front() + weights_.back()) / 2.0;
        }
        else
        {
            mean = std::accumulate(weights_.begin(), weights_.end(), 0.0) /  weights_.size(); // compute mean value from weights vector and return
        }
        return static_cast<unsigned int>(mean * mean);
    }

private:
    std::list<unsigned int> weights_;
};

class BlueBox : public Box
{
public:
    explicit BlueBox(double initial_weight) : Box(initial_weight) {}


    unsigned int getScore() const override
    {
        auto pairing = (small + big) * (small + big + 1) / 2 + small;
        return pairing;
    }

    unsigned int ​learnWeight(unsigned int _w) override
    {
        if ( _w < small)
        {
            small = _w;
        }
        if (big < _w)
        {
            big = _w;
        }
        return _w;
    }
private:
    unsigned int small{std::numeric_limits<unsigned int>::max()};
    unsigned int big{0};
};

std::unique_ptr<Box> Box::makeGreenBox(double initial_weight)
{
    return std::make_unique<GreenBox>(initial_weight);
}

std::unique_ptr<Box> Box::makeBlueBox(double initial_weight)
{
    return std::make_unique<BlueBox>(initial_weight);         
}

class Player
{
public:
    Player(std::vector<std::unique_ptr<Box>> &boxes) : boxes_(boxes) {}

    void takeTurn(unsigned int input_weight)
    {
        auto box = std::min_element(boxes_.begin(), // Compute Minimum Weight from Box List and Return Value from Box List 
                                    boxes_.end(),
                                    [](const auto &a, const auto &b)
                                    {
                                        return a->getWeight() < b->getWeight();
                                    });
        (*box)->​learnWeight(input_weight);  // Green Box And Blude Box
        score_ += (*box)->getScore(); // Save Player A or B Score
    }

    double getScore() const { return score_; }

private:
    std::vector<std::unique_ptr<Box>> &boxes_;
    double score_{0.0};
};

std::pair<double, double> play(const std::vector<unsigned int> &input_weights)
{
    std::vector<std::unique_ptr<Box>> boxes;
    boxes.emplace_back(Box::makeGreenBox(0.0));
    boxes.emplace_back(Box::makeGreenBox(0.1));
    boxes.emplace_back(Box::makeBlueBox(0.2));
    boxes.emplace_back(Box::makeBlueBox(0.3));

    Player player_A(boxes);
    Player player_B(boxes);

    for (auto i = 0; i < input_weights.size(); ++i)
    {
        if (i % 2 == 0)
            player_A.takeTurn(input_weights[i]);
        else
            player_B.takeTurn(input_weights[i]);
            
        std::cout << "Scores: player A " << player_A.getScore() << ", player B " << player_B.getScore() << std::endl;
    }

    return std::make_pair(player_A.getScore(), player_B.getScore());
}

TEST_CASE("Final scores for first 4 Fibonacci numbers", "[fibonacci4]")
{
    std::vector<unsigned int> inputs{1, 1, 2, 3, 5};
    auto result = play(inputs);
    REQUIRE(result.first == 13.0); 
    REQUIRE(result.second == 5.0);
}

TEST_CASE("Final scores for first 8 Fibonacci numbers", "[fibonacci8]")
{
    std::vector<unsigned int> inputs{1, 1, 2, 3, 5, 8, 13, 21};
    auto result = play(inputs);
    REQUIRE(result.first == 155.0);   // failed
    REQUIRE(result.second == 366.25); // failed
}

TEST_CASE("Test absorption of green box", "[green]")
{
    std::unique_ptr<Box> box = Box::makeGreenBox(0.0);
    REQUIRE(box->​learnWeight(1) == 1); // passed
    REQUIRE(box->getScore() == 1);      // passes
    REQUIRE(box->​learnWeight(2) == 3); // faild
    REQUIRE(box->getScore() == 5);      // failde
}

TEST_CASE("Test absorption of blue box", "[blue]")
{
    std::unique_ptr<Box> box = Box::makeGreenBox(0.0);
    REQUIRE(box->​learnWeight(1) == 1); // passed
    REQUIRE(box->getScore() == 3); // passed
    REQUIRE(box->​learnWeight(2) == 12); // failed
    REQUIRE(box->getScore() == 19); // failed
}

/*
int main()
{
    std::cout << "Hello world!" << std::endl;
    std::vector<unsigned int> inputs{1, 1, 2, 3, 5, 8, 13};
    auto result = play(inputs);

    return 0;
}
*/