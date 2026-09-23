#include "delta2.h"
#include <cassert>
#include <cmath>
#include <iostream>

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

    for(int i = 0; i < output_colors; i++) {
        this->aColors.push_back(i);
        this->allColors.push_back(i);
    }
    
    for(int i = output_colors; i < input_colors; i++) {
        this->bColors.push_back(i);
        this->allColors.push_back(i);
    }

    numberOfNodes = (input_colors * (long) pow(input_colors-1, 4) + input_colors * (long) pow(input_colors-1, 2)) / 2;
    numberOfEdges = input_colors * (long) pow(input_colors-1, 5) / 2;

    std::cout << "Number of Nodes: " << numberOfNodes << std::endl; 
    std::cout << "Number of Edges: " << numberOfEdges << std::endl; 

    int fdNodes = shm_open("nodes", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(ftruncate(fdNodes, sizeof(Node) * numberOfNodes) < 0) {
        std::cerr << "Error for nodes file" << std::endl;
    }
    int fdEdges = shm_open("edges", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(ftruncate(fdEdges, sizeof(Edge) * numberOfEdges) < 0) {
        std::cerr << "Error for edges file" << std::endl;
    }
    
    nodes = (Node*) mmap(NULL, sizeof(Node) * numberOfNodes, PROT_WRITE | PROT_READ, MAP_SHARED, fdNodes, 0);
    edges = (Edge*) mmap(NULL, sizeof(Edge) * numberOfEdges, PROT_WRITE | PROT_READ, MAP_SHARED, fdEdges, 0);
    close(fdNodes);
    close(fdEdges);
}

Delta2GraphCreator::~Delta2GraphCreator() {
    std::cout << "Destroy everything" << std::endl;
    munmap(nodes, sizeof(Node) * numberOfNodes);
    munmap(edges, sizeof(Edge) * numberOfEdges);
}

void Delta2GraphCreator::create_nodes() {
    long index = 0;
    for (auto center = allColors.begin(); center != allColors.end(); center++) {
        for (auto n_left = allColors.begin(); n_left != allColors.end(); n_left++) {
            if(n_left == center) continue;
            
            for (auto n_left_left = allColors.begin(); n_left_left != allColors.end(); n_left_left++) {
                if(n_left_left == n_left) continue;
                
                for (auto n_right = allColors.begin(); n_right != allColors.end(); n_right++) {
                    if(n_right < n_left || n_right == center) continue;
                    
                    for (auto n_right_right = allColors.begin(); n_right_right != allColors.end(); n_right_right++) {
                        if(n_right_right == n_right) continue;
                        if(n_left == n_right && *n_right_right < *n_left_left) continue;
                        
                        new (nodes + index) Node((uchar)*n_left_left, (uchar)*n_left, (uchar)*center, (uchar)*n_right, (uchar)*n_right_right);
                        index++;
                    }
                }
            }
        }
    }
    std::cout << "Finished creating " << index << " nodes!" << std::endl;
}

void Delta2GraphCreator::canonicalizeNode(Node& node) {
    if(node.right_ < node.left_ || (node.left_ == node.right_ && node.right_right_ < node.left_left_)) {
        node.reverse();
    }
}

void Delta2GraphCreator::findNeighborsOfSingleOrientedNode(Node* node, std::set<Node>& neighborNodes) {
    for(auto color = allColors.begin(); color != allColors.end(); color++) {
        if(*color != node->left_left_) {
            Node neiNode = node->shiftRight((uchar) *color);
            canonicalizeNode(neiNode);
            neighborNodes.insert(neiNode);
        }
        if(*color != node->right_right_) {
            Node neiNode = node->shiftLeft((uchar) *color);
            canonicalizeNode(neiNode);
            neighborNodes.insert(neiNode);
        }
    }
}

void Delta2GraphCreator::findNeighborsOfSingleNode(Node* node, std::set<Node>& neighborNodes) {
    Node reversedNode = node->getReversedNode();
    findNeighborsOfSingleOrientedNode(node, neighborNodes);
    findNeighborsOfSingleOrientedNode(&reversedNode, neighborNodes);
}



void Delta2GraphCreator::connect_nodes() {
    std::set<Node> neighorNodes;
    
    long index = 0;
    for(long i = 0; i < numberOfNodes; i++) {
        Node originalNode = nodes[i];
        findNeighborsOfSingleNode(&originalNode, neighorNodes);
        
        for(auto neiNode = neighorNodes.begin(); neiNode != neighorNodes.end(); neiNode++) {
            if(originalNode < *neiNode) {
                new (edges + index) Edge(originalNode, *neiNode);
                index++;
            }
        }
        neighorNodes.clear();
    }
    std::cout << "Finished creating " << index << " edges!" << std::endl;
}