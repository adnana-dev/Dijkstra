#include <bits/stdc++.h>
using namespace std;

typedef pair<double,int> ii;
typedef vector<double> vi;
typedef vector<bool> bi;
typedef vector< ii > vii;
#define INF 1e9
#define EPS 1e-6

// Creating a structure to represent a node in the heap 
struct node { 
	node* parent; // Parent pointer 
	node* child; // Child pointer 
	node* left; // Pointer to the node on the left 
	node* right; // Pointer to the node on the right 
	double key; // Value of the node 
	int vertex; //the vertex of the graph associated with the node
	int degree; // Degree of the node 
	char mark; // Black or white mark of the node 
	char c; // Flag for assisting in the Find node function 
}; 
vector<struct node*> pnode; 
// Declare an integer for number of nodes in the heap 
int no_of_nodes = 0; 

// Function to insert a node in heap 
struct node* insertion(struct node* mini, int v, double val) 
{ 
	struct node* new_node = (struct node*)malloc(sizeof(struct node)); 
	new_node->vertex=v;
	new_node->key = val; 
	new_node->degree = 0; 
	new_node->mark = 'W'; 
	new_node->c = 'N'; 
	new_node->parent = NULL; 
	new_node->child = NULL; 
	new_node->left = new_node; 
	new_node->right = new_node; 
	
	if (mini != NULL) { 
		(mini->left)->right = new_node; 
		new_node->right = mini; 
		new_node->left = mini->left; 
		mini->left = new_node; 
		if (new_node->key < mini->key) 
			mini = new_node; 
	} 
	else { 
		mini = new_node; 
	} 
	no_of_nodes++; 
	pnode[v]=new_node;
	return mini;
} 
// Linking the heap nodes in parent child relationship 
struct node* Fibonnaci_link(struct node* mini, struct node* ptr2, struct node* ptr1) 
{ 
    
	(ptr2->left)->right = ptr2->right; 
	(ptr2->right)->left = ptr2->left; 
	if (ptr1->right == ptr1) 
		mini = ptr1; 
	 
	ptr2->left = ptr2; 
	ptr2->right = ptr2; 
	ptr2->parent = ptr1; 
	 
	if (ptr1->child == NULL){ 
		 
		ptr1->child = ptr2;
	}	 
	 
	ptr2->right = ptr1->child; 
	 
	 
	ptr2->left = (ptr1->child)->left; 
	 
	((ptr1->child)->left)->right = ptr2; 
	(ptr1->child)->left = ptr2; 
	if (ptr2->key < (ptr1->child)->key) 
		ptr1->child = ptr2; 
	ptr1->degree++;
	 
	return mini; 
} 
// Consolidating the heap 
struct node* Consolidate(struct node* mini) 
{ 
	int temp1; 
	float temp2 = (log(no_of_nodes)) / (log(2)); 
	int temp3 = temp2; 
	struct node* arr[temp3+1]; 
	for (int i = 0; i <= temp3; i++) 
		arr[i] = NULL; 
	node* ptr1 = mini; 
	node* ptr2; 
	node* ptr3; 
	node* ptr4 = ptr1; 
	do { 
		ptr4 = ptr4->right; 
		temp1 = ptr1->degree; 
		while (arr[temp1] != NULL) { 
			ptr2 = arr[temp1]; 
			if (ptr1->key > ptr2->key) { 
				ptr3 = ptr1; 
				ptr1 = ptr2; 
				ptr2 = ptr3; 
			} 
			if (ptr2 == mini) 
				mini = ptr1; 
			
			mini=Fibonnaci_link(mini,ptr2, ptr1); 
			
			if (ptr1->right == ptr1) 
				mini = ptr1; 
			arr[temp1] = NULL; 
			temp1++; 
		} 
		arr[temp1] = ptr1; 
		ptr1 = ptr1->right; 
	} while (ptr1 != mini); 
	mini = NULL; 
	for (int j = 0; j <= temp3; j++) { 
		if (arr[j] != NULL) { 
			arr[j]->left = arr[j]; 
			arr[j]->right = arr[j]; 
			if (mini != NULL) { 
				(mini->left)->right = arr[j]; 
				arr[j]->right = mini; 
				arr[j]->left = mini->left; 
				mini->left = arr[j]; 
				if (arr[j]->key < mini->key) 
					mini = arr[j]; 
			} 
			else { 
				mini = arr[j]; 
			} 
			if (mini == NULL) 
				mini = arr[j]; 
			else if (arr[j]->key < mini->key) 
				mini = arr[j]; 
		} 
	}
	return mini;
} 

// Function to extract minimum node in the heap 
struct node* Extract_min(struct node* mini) 
{ 
	if (mini == NULL) 
		cout << "The heap is empty" << endl; 
	else { 
		node* temp = mini; 
		node* pntr; 
		pntr = temp; 
		node* x = NULL; 
		if (temp->child != NULL) { 

			x = temp->child; 
			do { 
				pntr = x->right; 
				(mini->left)->right = x; 
				x->right = mini; 
				x->left = mini->left; 
				mini->left = x; 
				if (x->key < mini->key) 
					mini = x; 
				x->parent = NULL; 
				x = pntr; 
			} while (pntr != temp->child); 
		} 
		
		(temp->left)->right = temp->right; 
		(temp->right)->left = temp->left; 
		
		mini = temp->right;
		 
		if (temp == temp->right && temp->child == NULL) 
			mini = NULL; 
		else { 
			mini = temp->right; 	
			mini=Consolidate(mini); 
			
		} 
		no_of_nodes--; 
		
	} 
	return mini;
} 

// Cutting a node in the heap to be placed in the root list 
struct node* Cut(struct node* mini,struct node* found, struct node* temp) 
{ 
	if (found == found->right) 
		temp->child = NULL; 

	(found->left)->right = found->right; 
	(found->right)->left = found->left; 
	if (found == temp->child) 
		temp->child = found->right; 

	temp->degree = temp->degree - 1; 
	found->right = found; 
	found->left = found; 
	(mini->left)->right = found; 
	found->right = mini; 
	found->left = mini->left; 
	mini->left = found; 
	found->parent = NULL; 
	found->mark = 'B'; 
	return mini;
} 

// Recursive cascade cutting function 
struct node* Cascase_cut(struct node* mini, struct node* temp) 
{ 
	node* ptr5 = temp->parent; 
	if (ptr5 != NULL) { 
		if (temp->mark == 'W') { 
			temp->mark = 'B'; 
		} 
		else { 
			mini=Cut(mini,temp, ptr5); 
			mini=Cascase_cut(mini,ptr5); 
		} 
	} 
	return mini;
} 

// Function to decrease the value of a node in the heap 
struct node* Decrease_key(struct node* mini, struct node* found, double val) 
{ 
	if (mini == NULL) 
		cout << "The Heap is Empty" << endl; 

	if (found == NULL) 
		cout << "Node not found in the Heap" << endl; 

	found->key = val; 

	struct node* temp = found->parent; 
	if (temp != NULL && found->key < temp->key) { 
		mini=Cut(mini,found, temp); 
		mini=Cascase_cut(mini,temp); 
	} 
	if (found->key < mini->key) 
		mini = found; 
	return mini;
} 


// Function to display the heap 
void display(struct node* mini) 
{ 
	node* ptr = mini; 
	if (ptr == NULL) 
		cout << "The Heap is Empty" << endl; 

	else { 
		cout << "The root nodes of Heap are: " << endl; 
		do { 
			cout << "("<<ptr->vertex<<","<<ptr->key<<")"; 
			ptr = ptr->right; 
			if (ptr != mini) { 
				cout << "-->"; 
			} 
		} while (ptr != mini && ptr->right != NULL); 
		cout << endl 
			<< "The heap has " << no_of_nodes << " nodes" << endl 
			<< endl; 
	} 
} 
void display1(struct node* mini)
{
	node *ptr=mini;
	if (mini !=NULL){
		cout << "("<<ptr->vertex<<","<<ptr->key<<")"<<endl; 
		cout <<"Left :";
		display1(mini->left);
		cout <<"Right :";
		display1(mini->right);
	}	
		
}	

int V,E;
double resd(DBL_MAX),resdf(DBL_MAX);
vector< vii > g;
vector< vii > gr;

 
vi ds;
vi dsf;

vi prec_d;
vi prec_df;
 
bi bdf;

void dijkstra_fibo(int s, int t)
{
	struct node* mini = NULL; 
	dsf[s]=0;
	
	prec_df[s]=s;
	mini=insertion(mini,s,0);
	int k=1;
	while(1)
	{
		int stop;
		
		
		double d=mini->key;
		int u=mini->vertex;
		
		
		if (u==t){
			resdf=d;
			break;
		}	
		mini=Extract_min(mini);
		for(int j=0; j<g[u].size(); j++)
		{
			ii v = g[u][j];
			if(dsf[u] + v.first < dsf[v.second])
			{
				dsf[v.second] = dsf[u] + v.first;
				if (bdf[v.second]==false){
					mini=insertion(mini,v.second, dsf[v.second]);
					bdf[v.second]=true;
				}	
				else{
					mini=Decrease_key(mini,pnode[v.second],dsf[v.second]);
				}	
				
				prec_df[v.second]=u;
			}
		}	
		
		k++;
	}
	return;
}

void dijkstra(int s, int t)
{
	ds[s]=0;
	priority_queue< ii, vector< ii >, greater<ii> > pqf; pqf.push(ii(0,s));
	prec_d[s]=s;
	int k=1;
	while(1)
	{
		int stop;
		 
		
		ii ff = pqf.top(); pqf.pop();
		int u(ff.second);
		double d(ff.first);
		if (u==t){
			resd=d;
			break;
		}	
		for(int j=0; j<g[u].size(); j++)
		{
			ii v = g[u][j];
			if(ds[u] + v.first < ds[v.second])
			{
				ds[v.second] = ds[u] + v.first;
				pqf.push(ii(ds[v.second],v.second));
				prec_d[v.second]=u;
			}
		}	
		 
		k++;
	}
	return;
}	
 
int main(int argc, char*argv[])
{
	int x,y;
	double wt;
	srand(time(nullptr)); 
	ifstream f;
	f.open(argv[1]);
	f >> V;
	f >> E;
	g.assign(V, vii());
	gr.assign(V, vii());
	ds.assign(V,INF);
	dsf.assign(V,INF);
	 
	bdf.assign(V,false);
	pnode.assign(V,NULL);
	prec_d.assign(V,-1);
	prec_df.assign(V,-1);
	for(int i=0; i<E; i++)
	{
		f >> x >> y >> wt;
		x--;y--;
		wt=wt+1+rand()%100;
		g[x].push_back(ii(wt,y));
		g[y].push_back(ii(wt,x));
		gr[x].push_back(ii(wt,y));
		gr[y].push_back(ii(wt,x));
	}
	int s,t;
	f >> s;
	f >> t;
	resd=DBL_MAX;
	s--; t--;
	dijkstra(s,t);
	if(resd == INF)
		cout << "-1\n";
	else
		cout << "valeur du plus court chemin de dijkstra "<<resd << "\n";
	cout<<"Le plus court chemin : ";
	int p=t;
	while (prec_d[p]!=p){
		cout<<p+1<<"-";
		p=prec_d[p];
	}
	cout<<p+1<<endl;
	resdf=DBL_MAX;
	dijkstra_fibo(s,t);
	if(resdf == INF)
		cout << "-1\n";
	else
		cout << "valeur du plus court chemin de dijkstra avec le tas de fibonnacci"<<resdf << "\n";
	cout<<"Le plus court chemin : ";
	p=t;
	while (prec_df[p]!=p){
		cout<<p+1<<"-";
		p=prec_df[p];
	}
	cout<<p+1<<endl;	
	 
	 
	fill(ds.begin(),ds.end(),INF);
	fill(dsf.begin(),dsf.end(),INF);
	f.close();
	return 0;
}
