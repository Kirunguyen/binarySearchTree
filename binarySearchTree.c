#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct node {
	int data;
	struct node *left, *right;
} node;

node *createNode(int x);
node* insert(node * root, int key);
node* deleteNode(node *root, int key);

void inorder(node * root);
void preorder(node *root);
void postorder(node *root);
void levelorder(node* root);

bool search(node *root, int key);
int size(node* root);
int height(node* root);
node* findMin(node* root);
node* findMax(node* root);

bool isBalanced(node* root);
void inorderToArray(node* root, node** nodes, int* pos);
node* arrayToBST(node** nodes, int start, int end);
void reBalance(node** root);

/*MAIN*/
int main() {
	node* root = NULL;
	root = insert(root, 50);
	root = insert(root, 30);
	root = insert(root, 70);
	root = insert(root, 20);
	root = insert(root, 40);
	root = insert(root, 60);
	root = insert(root, 80);

	printf("In-order traversal:\n");
	inorder(root);
	printf("\n");

	printf("Pre-order traversal:\n");
	preorder(root);
	printf("\n");

	printf("Post-order traversal:\n");
	postorder(root);
	printf("\n");

	printf("Level-order traversal:\n");
	levelorder(root);
	printf("\n");

	printf("Height: %d\n", height(root));
	printf("Size: %d\n", size(root));
	node* minNode = findMin(root);
	if (minNode) printf("Min: %d\n", minNode->data);
		else printf("Tree is empty.\n");

	node* maxNode = findMax(root);
	if (maxNode) printf("Max: %d\n", maxNode->data);
		else printf("Tree is empty.\n");
	printf("Is Balanced: %s\n", isBalanced(root) ? "True" : "False");

	printf("Search for 70: %s\n", search(root, 70)? "Found" : "Not found");
	printf("Search for 35: %s\n", search(root, 35)? "Found" : "Not found");

	root = deleteNode(root, 50);
	printf("Level-order after deletion: ");
	levelorder(root);
	printf("\n");

	reBalance(&root);
	printf("Level-order after re-balance: ");
	levelorder(root);

	return 0;
}
/*--END MAIN---*/

node *createNode(int x) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode -> data = x;
	newNode -> left = newNode -> right = NULL;
	return newNode;
}
//duyet giua -inorder
void inorder(node * root) {
	if(root != NULL) {
		inorder(root -> left);
		printf("%d\n", root -> data);
		inorder(root -> right);
	}
}

//duyet truoc -Preorder
void preorder(node *root) {
	if(root != NULL) {
		printf("%d\n", root -> data);
		preorder(root -> left);
		preorder(root -> right);
	}
}

//Duyet sau - Postorder
void postorder(node *root) {
	if(root != NULL) {
		postorder(root -> left);
		postorder(root -> right);
		printf("%d\n", root -> data);
	}
}

//Duyet theo muc -Level order
void levelorder(node* root) {
	if (root == NULL) return;
	node** queue = (node**)malloc(size(root) * sizeof(node*));
	int front = 0, rear = 0;
	queue[rear++] = root;
	while (front < rear) {
		node* current = queue[front++];
		printf("%d ", current->data);
		if (current->left) queue[rear++] = current->left;
		if (current->right) queue[rear++] = current->right;
	}
	free(queue);
}

bool search(node *root, int key) {
	if(root == NULL) {
		return false;
	} else if (root -> data == key) {
		return true;
	} else if (root -> data < key) {
		return search(root -> right, key);
	} else return search(root -> left, key);
}

int size(node* root) {
	if (!root) return 0;
	return size(root->left) + size(root->right) + 1;
}

node* insert(node * root, int key) {
	if(root == NULL) {
		return createNode(key);
	}
	if(key < root -> data) {
		root -> left = insert(root -> left, key);
	} else if(key > root -> data) {
		root -> right = insert(root -> right, key);
	}
	return root;
}

node* findMin(node* root) {
	if (root == NULL) return NULL;
	while (root->left != NULL) root = root->left;
	return root;
}

node* findMax(node* root) {
	if (root == NULL) return NULL;
	while (root->right != NULL) root = root->right;
	return root;
}

node* deleteNode(node *root, int key) {
	if(root == NULL) {
		return root;
	}
	if(key < root -> data) {
		root -> left = deleteNode(root -> left, key);
	} else if(key > root -> data) {
		root -> right = deleteNode(root -> right, key);
	} else {
		if(root -> left ==  NULL) {
			node *tmp = root -> right;
			free(root);
			return tmp;
		} else if(root -> right == NULL) {
			node *tmp = root -> left;
			free(root);
			return tmp;
		} else {
			node *tmp = findMin(root->right);
			root -> data = tmp -> data;
			root -> right = deleteNode(root -> right, tmp -> data);
		}
	}
	return root;
}

int height(node* root) {
	if (!root) return -1;
	int leftHeight = height(root->left);
	int rightHeight = height(root->right);
	return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

bool isBalanced(node* root) {
	if (!root) return true;
	int leftHeight = height(root->left);
	int rightHeight = height(root->right);
	return abs(leftHeight - rightHeight) <= 1 && isBalanced(root->left) && isBalanced(root->right);
}

void inorderToArray(node* root, node** nodes, int* pos) {
	if (root == NULL) return;
	inorderToArray(root -> left, nodes, pos);
	nodes[(*pos)++] = root;
	inorderToArray(root -> right, nodes, pos);
}

node* arrayToBST(node** nodes, int start, int end) {
	if (start > end) return NULL;
	int mid = (start + end) / 2;

	node* root = nodes[mid]; //array center = root

	root->left = arrayToBST(nodes, start, mid - 1);//moves left
	root->right = arrayToBST(nodes, mid + 1, end);//moves right

	return root;
}

void reBalance(node** root) {
	if (*root == NULL) return;
	int sizeBST = size(*root);

	//Convert BST -> sorted Array with "inorder"
	node** nodes = (node**)malloc(sizeBST * sizeof(node*));
	int pos = 0;

	inorderToArray(*root, nodes, &pos);

	//re-build BST from Array
	*root = arrayToBST(nodes, 0, sizeBST - 1);

	free(nodes);
}

