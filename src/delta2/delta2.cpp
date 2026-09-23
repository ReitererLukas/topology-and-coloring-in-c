#include "delta2.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>

// mmap
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

Delta2GraphCreator::Delta2GraphCreator(int input_colors, int output_colors) {
    assert(input_colors > output_colors);
    assert(input_colors < 100 && "Maximum number of 100 input colors");
    inputColors_ = input_colors;
    outputColors_ = output_colors;

    for(int i = 0; i < input_colors; i++) {
        this->allColors_.push_back(i);
    }

    numberOfNodes_ = (input_colors * (long) pow(input_colors-1, 4) + input_colors * (long) pow(input_colors-1, 2)) / 2;
    numberOfEdges_ = input_colors * (long) pow(input_colors-1, 5) / 2;

    std::cout << "Number of Nodes: " << numberOfNodes_ << std::endl; 
    std::cout << "Number of Edges: " << numberOfEdges_ << std::endl; 

    // int fdNodes = shm_open("nodes", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // if(ftruncate(fdNodes, sizeof(Node) * numberOfNodes_) < 0) {
    //     std::cerr << "Error for nodes file" << std::endl;
    // }
    
    // nodes_ = (Node*) mmap(NULL, sizeof(Node) * numberOfNodes_, PROT_WRITE | PROT_READ, MAP_SHARED, fdNodes, 0);
    // close(fdNodes);

    sat_ = new SAT(numberOfNodes_, numberOfEdges_, inputColors_, outputColors_);
}

Delta2GraphCreator::~Delta2GraphCreator() {
    std::cout << "Destroy everything" << std::endl;
    // munmap(nodes_, sizeof(Node) * numberOfNodes_);
}

long Delta2GraphCreator::create_nodes_parallel_helper(void* params) {
    ParamContainer* container = (ParamContainer*) params;
    Delta2GraphCreator* gc = (Delta2GraphCreator*) container->classObject_;

    return gc->create_nodes_parallel(container->center_);
}

long Delta2GraphCreator::create_nodes_parallel(int  center) const {
    long nodeCounter = 0;
    long edgeCounter = 0;
    for (auto n_left = allColors_.begin(); n_left != allColors_.end(); n_left++) {
        if(*n_left == center) continue;
        
        for (auto n_left_left = allColors_.begin(); n_left_left != allColors_.end(); n_left_left++) {
            if(n_left_left == n_left) continue;
            
            for (auto n_right = allColors_.begin(); n_right != allColors_.end(); n_right++) {
                if(n_right < n_left || *n_right == center) continue;
                
                for (auto n_right_right = allColors_.begin(); n_right_right != allColors_.end(); n_right_right++) {
                    if(n_right_right == n_right) continue;
                    if(n_left == n_right && *n_right_right < *n_left_left) continue;
                    
                    // Node* newNode = new (nodes_ + index) Node((uchar)*n_left_left, (uchar)*n_left, (uchar)*center, (uchar)*n_right, (uchar)*n_right_right);
                    Node newNode = Node((uchar)*n_left_left, (uchar)*n_left, (uchar) center, (uchar)*n_right, (uchar)*n_right_right);
                    sat_->addNode(newNode);
                    edgeCounter += connect_node(newNode);

                    nodeCounter++;
                }
            }
        }
    }
    return nodeCounter; // TODO: also return edge Counter and assert
}

void Delta2GraphCreator::create_nodes() {
    long index = 0;

    pthread_t* threads = (pthread_t*) calloc(allColors_.size(), sizeof(pthread_t));
    ParamContainer* containers = (ParamContainer*) calloc(allColors_.size(), sizeof(ParamContainer));

    for (auto center = allColors_.begin(); center != allColors_.end(); center++) {
        (containers + *center)->classObject_ = this;
        (containers + *center)->center_ = *center;
        pthread_create(threads + *center, NULL, (void *(*)(void *)) create_nodes_parallel_helper, (void*) (containers + *center));
    }

    for(long unsigned int i = 0; i < allColors_.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(containers);

    // assert(index == numberOfNodes_);
    std::cout << "Finished creating " << index << " nodes!" << std::endl;
}

void Delta2GraphCreator::canonicalizeNode(Node& node) const {
    if(node.right_ < node.left_ || (node.left_ == node.right_ && node.right_right_ < node.left_left_)) {
        node.reverse();
    }
}

void Delta2GraphCreator::findNeighborsOfSingleOrientedNode(Node& node, std::set<Node>& neighborNodes) const {
    for(auto color = allColors_.begin(); color != allColors_.end(); color++) {
        if(*color != node.left_left_) {
            Node neiNode = node.shiftRight((uchar) *color);
            canonicalizeNode(neiNode);
            neighborNodes.insert(neiNode);
        }
        if(*color != node.right_right_) {
            Node neiNode = node.shiftLeft((uchar) *color);
            canonicalizeNode(neiNode);
            neighborNodes.insert(neiNode);
        }
    }
}

void Delta2GraphCreator::findNeighborsOfSingleNode(Node& node, std::set<Node>& neighborNodes) const {
    Node reversedNode = node.getReversedNode();
    findNeighborsOfSingleOrientedNode(node, neighborNodes);
    findNeighborsOfSingleOrientedNode(reversedNode, neighborNodes);
}



long Delta2GraphCreator::connect_node(Node& originalNode) const {
    std::set<Node> neighorNodes;
    
    findNeighborsOfSingleNode(originalNode, neighorNodes);
    
    long counter = 0;
    for(auto neiNode = neighorNodes.begin(); neiNode != neighorNodes.end(); neiNode++) {
        if(originalNode < *neiNode) {
            sat_->addEdge(originalNode, neiNode);
            counter++;
        }
    }
    return counter;
}

void Delta2GraphCreator::analyzeStructure() {
    // std::set<int> idSet;
    // for(int i = 0; i < numberOfNodes_; i++) {
    //     idSet.insert(nodes_[i].getId(inputColors_));
    // }

    // std::cout << idSet.size() << std::endl;
    // std::cout << numberOfNodes_ << std::endl;
}


bool Delta2GraphCreator::solveSAT() {
    if(sat_->solve()) {
        std::cout << "SAT" << std::endl;
        return true;
    } else {
        std::cout << "UNSAT" << std::endl;
        return false;
    }
}