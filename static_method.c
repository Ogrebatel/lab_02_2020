#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLACK 1
#define RED 0

struct element;
typedef struct element element;


struct element {
    int value;
    element *small;
    element *big;
    element *parent;
    int type;
};


void preorder(element *p_element) {
    element *i;
    if (!p_element) {
        printf("Is empty");
    } else
        for (i = p_element; i; i = i->big) {
            printf("%d -> ", i->value);
            preorder(i->small);
        }
}

void inorder(element *p_element) {
    element *i;
    if (!p_element) {
        printf("Is empty");
    } else
        for (i = p_element; i; i = i->big) {
            inorder(i->small);
            printf("%d -> ", i->value);
        }
}

void postorder(element *p_element) {
    if (!p_element) {
        printf("Is empty");
    } else {
        postorder(p_element->small);
        postorder(p_element->big);
        printf("%d -> ", p_element->value);
    }
}

void print_tree(element *p_current_element, int counter) {
    int space_amount;

    if (p_current_element) {
        while (p_current_element) {
            if (p_current_element->big)
                print_tree(p_current_element->big, counter + 4);
            if (counter > 0) {
                space_amount = counter;
                do {
                    fputs(" ", stdout);
                    --space_amount;
                } while (space_amount);
            }
            p_current_element->type == BLACK ?
            printf("%d\n", p_current_element->value) :
            printf("<%d>\n", p_current_element->value);
            p_current_element = p_current_element->small;

            counter += 4;
        }
    } else {
        fputs("<Is Empty>", stdout);
    }
}

element* create_new_element(int val){
    element* temp = (element*)malloc(sizeof(element));
    temp->parent = NULL;
    temp->small = NULL;
    temp->big = NULL;
    temp->type = RED;
    temp->value = val;
    return temp;
}

bool is_left(element* node) { return node == node->parent->small; }

element *uncle_element(element* node) {
    if (node->parent == NULL || node->parent->parent == NULL)
        return NULL;

    if (is_left(node->parent))
        return node->parent->parent->big;
    else
        return node->parent->parent->small;
}
element *sibling_element(element* node) {
    if (node->parent == NULL)
        return NULL;

    if (is_left(node))
        return node->parent->big;

    return node->parent->small;
}

void move_down(element* node, element *nParent) {
    if (node->parent != NULL) {
        if (is_left(node)) {
            node->parent->small = nParent;
        } else {
            node->parent->big = nParent;
        }
    }
    nParent->parent = node->parent;
    node->parent = nParent;
}

bool has_red_child(element* node) {
    return (node->small != NULL && node->small->type == RED) ||
           (node->big != NULL && node->big->type == RED);
}
void double_black(element** root, element *x);

void left_turn(element** root, element *x) {
    element *nParent = x->big;

    if (x == *root)
        *root = nParent;
    move_down(x, nParent);

    x->big = nParent->small;
    if (nParent->small != NULL)
        nParent->small->parent = x;
    nParent->small = x;
}

void right_turn(element** root, element *x) {
    element *nParent = x->small;

    if (x == *root)
        *root = nParent;

    move_down(x, nParent);

    x->small = nParent->big;
    if (nParent->big != NULL)
        nParent->big->parent = x;
    nParent->big = x;
}

void swap_colors(element *x1, element *x2) {
    bool temp;
    temp = x1->type;
    x1->type = x2->type;
    x2->type = temp;
}

void swap_values(element *u, element *v) {
    int temp;
    temp = u->value;
    u->value = v->value;
    v->value = temp;
}

void fix_red_red(element** root, element *x) {
    if (x == *root) {
        x->type = BLACK;
        return;
    }
    element *parent = x->parent, *grandparent = parent->parent,
            *uncle = uncle_element(x);

    if (parent->type != BLACK) {
        if (uncle != NULL && uncle->type == RED) {
            parent->type = BLACK;
            uncle->type = BLACK;
            grandparent->type = RED;
            fix_red_red(root, grandparent);
        } else {
            if (is_left(parent)) {
                if (is_left(x)) {
                    swap_colors(parent, grandparent);
                } else {
                    left_turn(root, parent);
                    swap_colors(x, grandparent);
                }
                right_turn(root, grandparent);
            } else {
                if (is_left(x)) {
                    right_turn(root, parent);
                    swap_colors(x, grandparent);
                } else {
                    swap_colors(parent, grandparent);
                }
                left_turn(root, grandparent);
            }
        }
    }
}

element *successor(element *x) {
    element *temp = x;

    while (temp->small != NULL)
        temp = temp->small;

    return temp;
}

element *BSTreplace(element *x) {
    if (x->small != NULL && x->big != NULL)
        return successor(x->big);
    if (x->small == NULL && x->big == NULL)
        return NULL;
    if (x->small != NULL)
        return x->small;
    else
        return x->big;
}

void delete_element(element** root, element *v) {
    element *u = BSTreplace(v);
    bool uvBlack = ((u == NULL || u->type == BLACK) && (v->type == BLACK));
    element *parent = v->parent;

    if (u == NULL) {
        if (v == *root) {
            *root = NULL;
        } else {
            if (uvBlack) {
                double_black(root, v);
            } else {
                if (sibling_element(v) != NULL)
                    sibling_element(v)->type = RED;
            }
            if (is_left(v)) {
                parent->small = NULL;
            } else {
                parent->big = NULL;
            }
        }
        free(v);
        return;
    }

    if (v->small == NULL || v->big == NULL) {
        if (v == *root) {
            v->value = u->value;
            v->small = v->big = NULL;
            free(u);
        } else {
            if (is_left(v)) {
                parent->small = u;
            } else {
                parent->big = u;
            }
            free(v);
            u->parent = parent;
            if (uvBlack) {
                double_black(root, u);
            } else {
                u->type = BLACK;
            }
        }
        return;
    }
    swap_values(u, v);
    delete_element(root, u);
}

void double_black(element** root, element *x) {
    if (x == *root)
        return;

    element *sibling = sibling_element(x), *parent = x->parent;
    if (sibling == NULL) {
        double_black(root, parent);
    } else {
        if (sibling->type == RED) {
            parent->type = RED;
            sibling->type = BLACK;
            if (is_left(sibling)) {
                right_turn(root, parent);
            } else {
                left_turn(root, parent);
            }
            double_black(root, x);
        } else {
            if (has_red_child(sibling)) {
                if (sibling->small != NULL && sibling->small->type == RED) {
                    if (is_left(sibling)) {
                        sibling->small->type = sibling->type;
                        sibling->type = parent->type;
                        right_turn(root, parent);
                    } else {
                        sibling->small->type = parent->type;
                        right_turn(root, sibling);
                        left_turn(root, parent);
                    }
                } else {
                    if (is_left(sibling)) {
                        sibling->big->type = parent->type;
                        left_turn(root, sibling);
                        right_turn(root, parent);
                    } else {
                        sibling->big->type = sibling->type;
                        sibling->type = parent->type;
                        left_turn(root, parent);
                    }
                }
                parent->type = BLACK;
            } else {
                // 2 black children
                sibling->type = RED;
                if (parent->type == BLACK)
                    double_black(root, parent);
                else
                    parent->type = BLACK;
            }
        }
    }
}

element* search(element* root, int n) {
    element *temp = root;
    while (temp != NULL) {
        if (n < temp->value) {
            if (temp->small == NULL)
                break;
            else
                temp = temp->small;
        } else if (n == temp->value) {
            break;
        } else {
            if (temp->big == NULL)
                break;
            else
                temp = temp->big;
        }
    }

    return temp;
}

void insertion(element** root, int n) {
    element *newNode = create_new_element(n);
    if (*root == NULL) {

        newNode->type = BLACK;
        *root = newNode;
    } else {
        element *temp = search(*root, n);

        if (temp->value == n) {
            return;
        }
        newNode->parent = temp;

        if (n < temp->value)
            temp->small = newNode;
        else
            temp->big = newNode;
        fix_red_red(root, newNode);
    }
}


int main() {
    element **pp_root;
    int choice;
    int value;
    int flag = 1;

    pp_root = (element **) malloc(sizeof(element *));
    *pp_root = NULL;
    print_tree(*pp_root, 0);
    while (flag) {
        printf("\nWhat do you want to do?\n");
        printf("1.Insert\n");
        printf("2.Delete\n");
        printf("3.Show\n");
        printf("4.Inorder\n");
        printf("5.Preorder\n");
        printf("6.Postorder\n");
        printf("7.Exit\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\nEnter The Data for Inserting:");
                scanf("%d", &value);
                printf("\nInserting!\n\n");
                insertion(pp_root, value);
                break;
            case 2:
                printf("\nEnter The Data for Deleting:");
                scanf("%d", &value);
                delete_element(value, pp_root);
                break;
            case 3:
                print_tree(*pp_root, 0);
                break;
            case 4:
                printf("\n");
                inorder(*pp_root);
                printf("\n");
                break;
            case 5:
                printf("\n");
                preorder(*pp_root);
                printf("\n");
                break;
            case 6:
                printf("\n");
                if (*pp_root) {
                    postorder((*pp_root)->small);
                    postorder((*pp_root)->big);
                    printf("%d -> ", (*pp_root)->value);
                } else {
                    printf("Is empty");
                }
                printf("\n");
                break;
            case 7:
                printf("\nExiting......");
                flag = 0;
            default:
                printf("Please Enter a valid number!!\n");
                break;
        }
    }
    return 0;
}
