#include<stdio.h>
#include<stdlib.h>

typedef struct treeNode * treeNode;
struct treeNode{
  int ra;
  int grade;
  int height;

  treeNode nextLeft;
  treeNode nextRight;
};

treeNode createNode(int ra, int grade){
  treeNode newNode = (treeNode) malloc(sizeof(struct treeNode));

  newNode->ra    = ra;
  newNode->grade = grade;
  newNode->height = 0;
  newNode->nextLeft = NULL;
  newNode->nextRight = NULL;

  return newNode;
}


int max(int a, int b){
  return a > b ? a : b;
}

int getHeight(treeNode node){
    return node? node->height : -1;
}

int updateHeight(treeNode node){
  if(node){
    node->height = 1 + max(getHeight(node->nextLeft), getHeight(node->nextRight));
  }
}

void printInOrder(treeNode root){
  if(root){
    printInOrder(root->nextLeft);
    printf("%d ", root->ra);
    printInOrder(root->nextRight);
  }
}

int getBalanceFactor(treeNode root){
  int factor;
  if(root){
    int hL = getHeight(root->nextLeft);
    int hR = getHeight(root->nextRight);
    factor = hL - hR;
  }else{
    factor = 0;
  }

  return factor;
}

int isBalanced(treeNode root){
  int factor = getBalanceFactor(root);
  return factor >= -1 && factor <= 1;
}

void rotateLL(treeNode * root){
  treeNode pivot = (*root)->nextLeft;

  (*root)->nextLeft = pivot->nextRight;
  pivot->nextRight = (*root);

  *root = pivot;
}

void rotateRR(treeNode * root){
  treeNode pivot = (*root)->nextRight;

  (*root)->nextRight = pivot->nextLeft;
  pivot->nextLeft = (*root);

  *root = pivot;
}

void rotateLR(treeNode * root){
  treeNode aux = (*root)->nextLeft;
  treeNode pivot = aux->nextRight;

  rotateRR(&((*root)->nextLeft));
  rotateLL(root);
}

void rotateRL(treeNode * root){
  treeNode aux = (*root)->nextRight;
  treeNode pivot = aux->nextLeft;

  rotateLL(&((*root)->nextRight));
  rotateRR(root);
}

void rotate(treeNode * root){
	char c1, c2 ;
	int x, y, z;

	int balanceFactor = getBalanceFactor(*root);
	int childBalanceFactor;
	if(balanceFactor >= 0){
	  	c2 = 'D';
	    childBalanceFactor = getBalanceFactor((*root)->nextLeft);
	    if(childBalanceFactor >= 0){
	      c1 = 'S';
	      rotateLL(root);
	    }else{
	      c1 = 'D';
	      rotateLR(root);
	    }

	}else{
	    c2 = 'E';
	    childBalanceFactor = getBalanceFactor((*root)->nextRight);
	    if(childBalanceFactor <= 0){
	      c1 = 'S';
	      rotateRR(root);
	    }else{
	      c1 = 'D';
	      rotateRL(root);
	    }
	}

	y = (*root)->ra;
	x = (*root)->nextLeft->ra;
	z = (*root)->nextRight->ra;
	printf("[Rotacao: %c%c]\n", c1, c2);
	printf("[x=%d y=%d z=%d]\n", x, y, z);
}

void balance(treeNode * root){
  printf("[No desbalanceado: %d]\n", (*root)->ra);
  rotate(root);

  updateHeight((*root)->nextLeft);
  updateHeight((*root)->nextRight);
  updateHeight(*root);
}

int insertNode(treeNode * root, treeNode newNode){
  int rotated = 0;
  if(*root){
    if((*root)->ra == newNode->ra){
      (*root)->grade = newNode->grade;
      free(newNode);
    }else{
      if((*root)->ra > newNode->ra){
        rotated = insertNode(&(*root)->nextLeft, newNode);
      }else{
        rotated = insertNode(&(*root)->nextRight, newNode);
      }

      if(!rotated){
        updateHeight(*root);

        if(!isBalanced(*root)){
          balance(root);

          rotated = 1;
        }
      }
    }
  }else{
    *root = newNode;
  }

  return rotated;
}

void insertRa(treeNode * root, int ra, int grade){
  treeNode newNode = createNode(ra, grade);
  int alreadyBalanced = !insertNode(root, newNode);

  if(alreadyBalanced){
    printf("[Ja esta balanceado]\n");
  }
}

int deleteNode(treeNode * root, int ra){
  int rotated = 0;

  if(!(*root)){
    return rotated;
  }

  if((*root)->ra == ra){
    if((*root)->nextLeft && (*root)->nextRight){
      treeNode tmp = (*root)->nextRight;
      while(tmp->nextLeft){
        tmp = tmp->nextLeft;
      }

      (*root)->ra = tmp->ra;
      (*root)->grade = tmp->grade;
      rotated = deleteNode(&(*root)->nextRight, (*root)->ra);
    }else{
      treeNode tmp;

      if((*root)->nextLeft){
        tmp = (*root)->nextLeft;
      }else{
        tmp = (*root)->nextRight;
      }

      treeNode deleteMe = *root;
      *root = tmp;

      free(deleteMe);
    }
  }else{
    if(ra < (*root)->ra){
      rotated = deleteNode(&(*root)->nextLeft, ra);
    }else{
      rotated = deleteNode(&(*root)->nextRight, ra);
    }
  }
  
  updateHeight(*root);
  if(!isBalanced(*root)){
    balance(root);
    rotated = 1;
  }

  return rotated;
}

void deleteRa(treeNode * root, int ra){
	int alreadyBalanced = !deleteNode(root, ra);
  if(alreadyBalanced){
    printf("[Ja esta balanceado]\n");
  }
}

void clearTree(treeNode * root){
	if(*root){
		clearTree(&(*root)->nextLeft);
		clearTree(&(*root)->nextRight);

		free(*root);
	}

	*root = NULL;
}

void printTreeHeight(treeNode root){
	int h = getHeight(root);
	printf("A=%d\n", h);
}

void posOrderRecursion(treeNode root){
	if(root){
		posOrderRecursion(root->nextLeft);
		posOrderRecursion(root->nextRight);
		printf("%d ", root->ra);
	}
}

void printPosOrder(treeNode root){
	printf("[");
	posOrderRecursion(root);
	printf("]\n");
}

void printTopDownAux(treeNode root, int h){
  if(h == 0){
    if(root){
      printf("%d(%d) ", root->ra, root->grade);
    }else{
      printf("- ");
    }
  }else{
    if(root){
      printTopDownAux(root->nextLeft,  h-1);
      printTopDownAux(root->nextRight, h-1);
    }else{
      printTopDownAux(NULL, h-1);
      printTopDownAux(NULL, h-1);
    }
  }
}

void printTopDown(treeNode root){
  int h = getHeight(root);
  for(int i = 0; i <= h; i++){
    for(int j = 0; j < h-i; j++){
      printf(" ");
    }
    printTopDownAux(root, i);
    printf("\n");
  }
}

void printGrade(treeNode root, int ra){
	int i = 0;

	//search grade
	treeNode tmp = root;
	while(tmp){
		i++;
		if(ra < tmp->ra){
			tmp = tmp->nextLeft;
		}else if(ra > tmp->ra){
			tmp = tmp->nextRight;
		}else{
			//found it
			break;
		}
	}

	printf("C=%d Nota=%d\n", i, tmp? tmp->grade : -1);
}

int main(void){
  treeNode root = NULL;

  char c;
  int ra, grade;

  scanf("%c", &c);
  while(1){
    if(c >= 'a' && c <= 'z') c += 'A'-'a';
    switch(c){
      case 'I':
        scanf("%d %d", &ra, &grade);
        insertRa(&root, ra, grade);
        break;
      case 'R':
        scanf("%d", &ra);
        deleteRa(&root, ra);
        break;
      case 'B':
      	scanf("%d", &ra);
      	printGrade(root, ra);
      	break;
      case 'A':
      	printTreeHeight(root);
      	break;
      case 'P':
        printPosOrder(root);
		    clearTree(&root);
        exit(0);
        break;
      case 'T':
      	printTopDown(root);
      	break;
    }
    scanf("\n%c", &c);
  }
  return 0;
}
