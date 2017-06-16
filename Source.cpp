#include<gl/GLUT.H>
#include<math.h>
#include<stdio.h>
#include"stdlib.h"

int a[3][101], p[3] = { 0 };
int q[3] = { -1,-1,-1 };
int r[3] = { -1, -1, -1 };
long int time[3] = { 0 };

int width = 1300;
int height = 600;

char temp[100];
bool paused = false;

long int rate = 16;

void doBubbleSort(int);
void doSelectionSort(int);
void doMergeSort(int);


inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }

class Item {
private:
	int lo;
	int hi;
	Item* prev;
public:
	void set(int low, int high, Item* bef) {
		lo = low;
		hi = high;
		prev = bef;
	}
	void get(int* low, int* high, Item** bef) {
		*low = lo;
		*high = hi;
		*bef = prev;
	}
};

class Stack {
private:
	int size;
	Item* top;
public:
	Stack() {
		size = 0;
	}

	void push(int x, int y) {
		Item* i = new Item;
		if (size == 0) {
			i->set(x, y, NULL);
		}
		else {
			i->set(x, y, top);
		}
		top = i;
		size++;
	}
	void pop(int *x, int *y) {
		if (size == 0) {
			*x = -1;
			*y = -1;
		}
		else {
			top->get(x, y, &top);
			size--;
		}
	}
};

void Font(void *font, char *text, int x, int y)
{
	glRasterPos2i(x, y);
	while (*text != '\0')
	{
		glutBitmapCharacter(font, *text);
		++text;
	}
}

void doQuickSort(int index){
	static int min, max=99;
	static Stack s;
	static int i, j, piv;
	static bool firstRun = true, nextStep = false;
	bool changed = false;
	if (firstRun){
		min = 0; max = 99;
		i = min + 1;
		j = max;
		piv = min;
		p[index] = i;
		q[index] = j;
		r[index] = piv;
		firstRun = false;
	}
	if (nextStep){
		s.pop(&min, &max);
		nextStep = false;
		i = min + 1;
		j = max;
		piv = min;
		r[index] = piv;
	}

	if (min >= 0 && max <= 99 && min != max){
		time[index] += rate;
		if (a[index][i] <= a[index][piv] && i <= max){
			i++; changed = true; p[index] = i;
		}
		if (a[index][j] >= a[index][piv] && j>min){
			j--; changed = true; q[index] = j;
		}
		if (!changed){
			if (i <= j){
				int t = a[index][j];
				a[index][j] = a[index][i];
				a[index][i] = t;
			}
			else{
				int t = a[index][piv];
				a[index][piv] = a[index][j];
				a[index][j] = t;
				r[index] = j;
				if (j+1<max)
					s.push(j+1,max);
				if (j-1>min)
					s.push(min, j - 1);
				nextStep = true;
			}
		}
	}
	else{
		p[index] = -1;
		q[index] = -1;
		r[index] = -1;
	}
}

void doBubbleSort(int index) {
	static int i = 0, j = 0;
	if (i < 100) {
		if (j < 100 - i - 1) {
			if (a[index][j] > a[index][j + 1]) {
				q[index] = j;
				int t = a[index][j];
				a[index][j] = a[index][j + 1];
				a[index][j + 1] = t;
			}
			j++;
		}
		else {
			j = 0;
			i++;
		}
		p[index] = j;
		time[index] += rate;
	}
	else q[index] = -1;
}

void doSelectionSort(int index) {
	static int i = 0, j = 0;
	static int minpos = 0;
	static int min = 101;
	if (i < 100) {
		if (j < 100) {
			if (a[index][j] < min) {
				min = a[index][j];
				minpos = j;
			}
			p[index] = j;
			j++;
		}
		else {
			int t = a[index][minpos];
			a[index][minpos] = a[index][i];
			a[index][i] = t;
			p[index] = i;
			j = ++i;
			min = 101;
		}
		q[index] = minpos;
		time[index] += rate;
	}
	else p[index] = q[index] = -1;
}

void doMergeSort(int index) {
	static Stack s;
	static bool firstRun = true;
	static int lo, hi, mid;
	static int Lmax, Hmax;
	int max(int, int);
	int min(int, int);
	void merge(int, int, int, int);

	static bool isMerge = false;

	if (!isMerge){
		s.pop(&lo, &hi);
		p[index] = lo;
		q[index] = hi;
		if ((lo == -1 || hi == -1) && firstRun) { //first run code
			lo = 0; hi = 100;
			s.push(lo, hi);
			mid = (lo + hi) / 2;
			s.push(mid, hi);
			s.push(lo, mid);
			firstRun = false;
		}
		else if (hi - lo < 2) {
			Lmax = min(Lmax, lo);
			Hmax = max(Hmax, hi);
			isMerge = true; return;//merge(lo, mid, hi, index);
		}
		else {
			if (lo >= Lmax && hi <= Hmax){ isMerge = true; return; } //merge(lo, mid, hi, index);
			else {
				mid = (lo + hi) / 2;
				s.push(lo, hi);
				s.push(mid, hi);
				s.push(lo, mid);
			}
		}
	}
	else{
		//merge code
		static int b[101];
		static bool firstTime = true;
		static int i = lo, j = lo, k = (lo+hi)/2;

		if (firstTime){
			i = lo, j = lo, k = (lo+hi)/2;
			r[index] = i;
			for (int i = 0; i <= 100; i++){
				b[i] = a[index][i];
			}
			firstTime = !firstTime;
		}
		if (i <= hi && lo<hi){
			if (j < (lo+hi)/2 && (k >= hi || b[j] <= b[k])){
				a[index][i] = b[j++];
			}
			else{
				a[index][i] = b[k++];
			}
			i++;
			r[index] = i;
		}
		else { 
			isMerge = false; 
			firstTime = true;
			r[index] = -1;
		}
	}

	if (lo != -1 && hi != -1)
		time[index] += rate;
}

void merge(int lo, int mid, int hi, int index) {
	if (lo == hi) return;
	int b[101];
	int i, j, k;
	i = lo;
	j = lo;
	k = (lo + hi) / 2;

	for (i = lo; i <= hi; i++) {
		if (j < (lo + hi) / 2 && (k >= hi || a[index][j] <= a[index][k])) {
			b[i] = a[index][j++];
		}
		else {
			b[i] = a[index][k++];
		}
	}

	for (i = lo; i <= hi; i++) a[index][i] = b[i];
}

void printArrayValues(int index) {

	glTranslated(0, index * (100 + 10), 0);
	glColor3f(1, 1, 1);
	switch (index) {
	case 0:Font(GLUT_BITMAP_HELVETICA_18, "Bubble Sort", 10, 90);
		break;
	case 1:Font(GLUT_BITMAP_HELVETICA_18, "QuickSort", 10, 90);
		break;
	case 2:Font(GLUT_BITMAP_HELVETICA_18, "Merge Sort", 10, 90);
		break;
	}
	for (int i1 = 0; i1 < 100; i1++) {

		if (i1 == p[index]) glColor3f(0, 1, 0);
		else if (i1 == q[index]) glColor3f(0, 0, 1);
		else if (i1 == r[index]) glColor3f(1, 1, 0);
		else glColor3f(1, 0, 0);

		float x1 = 10 * i1;
		float x2 = x1 + 10;
		float y = (float)a[index][i1];
		glBegin(GL_POLYGON);
		glVertex2f(x1, 0);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
		glVertex2f(x2, 0);
		glEnd();
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, 0);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
		glVertex2f(x2, 0);
		glEnd();
	}
	glLoadIdentity();
}

void display() {

	sprintf_s(temp, "Delay: %dms, Bubble Sort Time: %dms, QuickSort time: %dms, Merge Sort Time: %dms", rate,time[0],time[1],time[2]);
	glClear(GL_COLOR_BUFFER_BIT);
	printArrayValues(0);
	printArrayValues(1);
	printArrayValues(2);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP); //pause Button
		glVertex2f(0, -15);
		glVertex2f(0, -5);
		glVertex2f(40,-5);
		glVertex2f(40, -15);
	glEnd();
	Font(GLUT_BITMAP_HELVETICA_12, paused?"Resume":"Pause", 5, -12);
	Font(GLUT_BITMAP_HELVETICA_18, temp, 50, -10);
	glFlush();
	glLoadIdentity();
}

void myInit() {
	int rand();

	for (int i = 0; i < 100; i++) {
		a[0][i] = rand() % 100;
		a[1][i] = a[0][i];
		a[2][i] = a[0][i];
		printf("%d ", a[0][i]);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10, 1010, -30, 340);
	glMatrixMode(GL_MODELVIEW);

}

void timer(int i) {
	doBubbleSort(0);
	//doSelectionSort(1);
	doMergeSort(2);
	doQuickSort(1);
	display();
	if (!paused)
		glutTimerFunc(rate, timer, 1);
}

void keys(unsigned char ch, int x, int y) {
	switch (ch) {
	case '-':rate *= 2;
		break;
	case '+':rate /= 2;
		break;
	case ' ': paused = !paused;
		break;
	case 'n': paused = true;
		timer(1);
		break;
	}
	if (rate == 0) rate = 1;
	if (!paused) timer(1);
	display();
}

void mouse(int button,int state, int x, int y) {
	float x1, y1,x2,y2;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		x1 = x / (float)width;
		y1 = y / (float)height;
		x2 = -10 + 1000 * x1; 
		y2 = 340 + (-370)*y1;
		if ((x2 >= 0 && x2 <= 40) && (y2 >= -15 && y2 <= -5)) { 
			paused = !paused;	
			timer(1);
		}
	}
}

void reshape(int w, int h){
	width = w;
	height = h;
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glViewport(0, 0, height, width);
	glutCreateWindow("Comparative Visualization of Sorting Techniques");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000, timer, 1);
	myInit();
	glutMainLoop();
}
