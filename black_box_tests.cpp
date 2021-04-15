//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Vojtech Eichler <xeichl01@stud.fit.vutbr.cz>
// $Date:       $2017-02-26
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Vojtech Eichler
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
class NonEmptyTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int val[] = {3, 27, 12, 60, 33, 18, 21, 6, 15, 9, 24, 30};
        for (int i = 0; i < 12; i++) {
            tree.InsertNode(val[i]);
        }
    }

    BinaryTree tree;
};

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

class TreeAxioms : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int val[] = {3, 27, 12, 60, 33, 18, 21, 6, 15, 9, 24, 30};
        for (int i = 0; i < 12; i++) {
            tree.InsertNode(val[i]);
        }
    }

    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode) {
    //testing if tree is empty
    ASSERT_EQ(tree.GetRoot(), nullptr);

    //testing inserting first value
    std::pair<bool, Node_t*> p = tree.InsertNode(3);

    //using assert, trying inserting already inserted value later
    //wouldn't make sense if these two didn't go through
    ASSERT_TRUE(p.first);
    ASSERT_NE(p.second, nullptr);

    //testing insert when a node with the same value already exists
    p = tree.InsertNode(3);
    EXPECT_FALSE(p.first);
    EXPECT_NE(p.second, nullptr);
}

TEST_F(EmptyTree, DeleteNode) {
    ASSERT_EQ(tree.GetRoot(), nullptr);

     //testing if DeleteNode returns false
    EXPECT_FALSE(tree.DeleteNode(3));
    EXPECT_EQ(tree.GetRoot(),nullptr);
}

TEST_F(EmptyTree, FindNode) {
    ASSERT_EQ(tree.GetRoot(), nullptr);
    EXPECT_EQ(tree.FindNode(3), nullptr);
}

TEST_F(NonEmptyTree, InsertNode) {
    //inserting a value that already exists in the tree
    std::pair<bool, Node_t*> p = tree.InsertNode(3);
    EXPECT_FALSE(p.first);
    //p.second should point to already existing node
    EXPECT_NE(p.second, nullptr);

    //attempting inserting new value
    p = tree.InsertNode(5);
    EXPECT_TRUE(p.first);
    EXPECT_NE(p.second, nullptr);
}

TEST_F(NonEmptyTree, DeleteNode) {
    //deleting nonexisting value
    EXPECT_FALSE(tree.DeleteNode(5));

    EXPECT_TRUE(tree.DeleteNode(3));

    //deleting all
    int val[] = {30, 24, 9, 15, 6, 21, 18, 33, 60, 12, 27, 3};
    for (int i = 0; i < 12; i++) {
        tree.DeleteNode(val[i]);
    }
    EXPECT_EQ(tree.GetRoot(), nullptr);
}

TEST_F(NonEmptyTree, FindNode) {
    //looking for a value that isn't inside a tree
    EXPECT_EQ(tree.FindNode(5), nullptr);

    //number 3 should be in a tree
    EXPECT_NE(tree.FindNode(3), nullptr);

    //trying to find all values
    int val[] = {30, 24, 9, 15, 6, 21, 18, 33, 60, 12, 27, 3};
    for (int i = 0; i < 12; i++) {
        Node_t *node = tree.FindNode(val[i]);
        //aborting test if node is null, so that the next test does'nt segfault
        ASSERT_NE(node, nullptr);
        EXPECT_EQ(node->key, val[i]);
    }
}

TEST_F(TreeAxioms, Axiom1) {
    std::vector<Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);
    //all leaves should be black, leafNodes is emptied when .GetLeafNodes
    //is called, in that case, for would just end -> tree has no leaves
    for (auto node : leafNodes) {
        EXPECT_EQ(node->color, BinaryTree::BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2) {
    std::vector<Node_t *> allNodes;
    //inserting all nodes from a tree into an array
    tree.GetAllNodes(allNodes);
    //cycling through all nodes
    for (auto node : allNodes) {
        if (node->color == BinaryTree::RED) {
            //if there's a child to a red node, then it has to be black
            if (node->pLeft != nullptr) 
                EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
            if (node->pRight != nullptr)
                EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3) {
    Node_t *node = nullptr;
    size_t n = 0;
    std::vector<Node_t *> leafNodes;
    
    tree.GetLeafNodes(leafNodes);
    //Looking for number of black nodes on the way from first leaf node to root
    node = leafNodes[0]->pParent;
    while (node != tree.GetRoot()) {
        if (node->color == BinaryTree::BLACK)
            n++;
        node = node->pParent;
    }

    for (auto leaf : leafNodes) {
        size_t m = 0;
        leaf = leaf->pParent;
        while (leaf != tree.GetRoot()) {
            if (leaf->color == BinaryTree::BLACK)
                m++;
            leaf = leaf->pParent;
        }
        //comparing black node count for each leaf
        EXPECT_EQ(n, m);
    }
}

/*** Konec souboru black_box_tests.cpp ***/
