#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type = BST;
static tree root;

struct tree_node {
    char *key;
    tree_colour colour;
    int frequency;
    tree left;
    tree right;
};

/*****************************************************************************
 * Function: tree_insert
 * Input   : tree t, char *str
 * Output  : tree
 * Procedure: Allocate memory for a new tree if str is not found, call function
              tree_fix to re-adjust the tree post-insertion.
 *****************************************************************************/

tree tree_insert(tree t, char *str) {
     if(t == NULL || t->key == NULL) {
        t = tree_new(tree_type);
        t->key = emalloc((strlen(str)+1) * sizeof(char*));
        strcpy(t->key,str);
        t->frequency = 1;
        return t;
     }else if(strcmp(t->key,str) == 0){
          /* this line below allows for adding duplicates into tree */
         t->frequency++;
     }else if(strcmp(t->key,str) > 0){
         t->left = tree_insert(t->left, str);
     }else if(strcmp(t->key,str) < 0) {
         t->right = tree_insert(t->right, str);
     }
     if (tree_type == RBT) {
         t = tree_fix(t);
     }
     return t;
}


/*****************************************************************************
 * Function: root_fix
 * Input   : tree t
 * Output  : tree
 * Procedure: Recolours the root to conform to an RBT
 *****************************************************************************/

tree root_fix(tree t){
    t->colour = BLACK;
    return t;
}

/*****************************************************************************
 * Function: right_rotate
 * Input   : tree t
 * Output  : tree
 * Procedure: Right rotates the rbt t
 *****************************************************************************/

tree right_rotate(tree t){
    tree temp;
    temp = t;
    t = t->left;
    temp->left = t->right;
    t->right = temp;
    return t;
}

/*****************************************************************************
 * Function: left_rotate
 * Input   : tree t
 * Output  : tree
 * Procedure: Left rotates the rbt t
 *****************************************************************************/

tree left_rotate(tree t){
    tree temp;
    temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    return t;
}

/*****************************************************************************
 * Function: tree_fix
 * Input   : tree t
 * Output  : tree
 * Procedure: Adjusts tree so it conforms to the conditions of an rbt
 *****************************************************************************/

tree tree_fix(tree t){
    if (IS_RED(t->left)){
        if(IS_RED(t->left->left)){
            if(IS_RED(t->right)){
                t->colour = RED;
                t->left->colour = BLACK;
                t->right->colour = BLACK;
            }
            else{
                t = right_rotate(t);
                t->colour = BLACK;
                t->right->colour = RED;
            }
        }else if (IS_RED(t->left->right)){
            if (IS_RED(t->right)){
                t->colour = RED;
                t->left->colour = BLACK;
                t->right->colour = BLACK;
            }else{
                t->left = left_rotate(t->left);
                t = right_rotate(t);
                t->colour = BLACK;
                t->right->colour = RED;
            }
        }
    }if (IS_RED(t->right)){
        if (IS_RED(t->right->left)){
            if(IS_RED(t->left)){
                t->colour = RED;
                t->left->colour = BLACK;
                t->right->colour = BLACK;
            }else{
                t->right = right_rotate(t->right);
                t = left_rotate(t);
                t->colour = BLACK;
                t->left->colour = RED;
            }
        }else if (IS_RED(t->right->right)){
            if(IS_RED(t->left)){
                t->colour = RED;
                t->left->colour = BLACK;
                t->right->colour = BLACK;
            }else{
                t = left_rotate(t);
                t->colour = BLACK;
                t->left->colour = RED;
            }
        }
    }
    root = root_fix(root);
    return t;
}

/*****************************************************************************
 * Function: tree_inorder
 * Input   : tree t, void f(int freq char *str)
 * Output  : void
 * Procedure: Traverses and prints out tree inorder to stdout
*****************************************************************************/

void tree_inorder(tree t, void f(int freq, char *str)) {
    if(t->key == NULL) return;
    if(t->left != NULL) {
        tree_inorder(t->left, f);
    }
    if((tree_type == RBT)) {
        if(IS_BLACK(t)){
            f(t->frequency, t->key);
        }else{
            f(t->frequency, t->key);
        }
    }else{
        f(t->frequency, t->key);
    }
    if(t->right != NULL) {
        tree_inorder(t->right, f);
    }
}

/*****************************************************************************
 * Function: tree_preorder
 * Input   : rbt t, void f(int freq char *str)
 * Output  : void
 * Procedure: Traverses and prints out tree preorder to stdout
*****************************************************************************/

void tree_preorder(tree t, void f(int freq, char *str)){
    if (t == NULL) return;
    if((tree_type == RBT)) {
        if(IS_BLACK(t)){
            f(t->frequency, t->key);
            
        }else{
            f(t->frequency, t->key);
        }
    }else{
        f(t->frequency, t->key);
    }
    tree_preorder(t->left,f);
    tree_preorder(t->right, f);
}

/*****************************************************************************
* Function: tree_new
* Input   : tree_t t
* Output  : tree
* Procedure: Allocates memory to the tree;
*****************************************************************************/

tree tree_new(tree_t t){
    tree result;
    tree_type = t;
    result = emalloc(sizeof *result);
    result->key = NULL;
    result->colour = RED;
    result->left = NULL;
    result->right = NULL;
    result->frequency = 0;
    if(root == NULL){
        root = result;
    };
    return result;
    
}

/*****************************************************************************
 * Function: tree_search
 * Input   : tree t, char *str
 * Output  : int
 * Procedure: Checks if the tree is empty, if not; traverse the nodes comparing
              the str to each nodes str. Return 1 if found, 0 if not.
 *****************************************************************************/

int tree_search(tree t, char *str) {
    if (t == NULL) return 0;
    if (strcmp(t->key, str) == 0) return 1;
    else if(strcmp(t->key, str) > 0) {
        return tree_search(t->left, str);
    }else{
        return tree_search(t->right, str);
    }return 0;
}

/*****************************************************************************
 * Function: tree_free
 * Input   : tree t
 * Output  : tree
 * Procedure: Traverse tree and free the memory allocated to each node.
 *****************************************************************************/

tree tree_free(tree t){
    if (t == NULL) return t;
    if (t->left != NULL){
        tree_free(t->left);
    }
    if (t->right != NULL){
        tree_free(t->right);
    }
    free(t);
    return t;
}

/*****************************************************************************
 * Function: tree_depth
 * Input   : tree t
 * Output  : int
 * Procedure: Traverse rbt and return the depth.
 *****************************************************************************/

int tree_depth(tree t) {
    int left_depth, right_depth;
    if (t == NULL) return 0;
    left_depth = tree_depth(t->left);
    right_depth = tree_depth(t->right);
    return (left_depth > right_depth) ? ++left_depth : ++right_depth;
}


/***************************************************************************
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 ***************************************************************************/
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}


/****************************************************************************
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 ***************************************************************************/
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}
