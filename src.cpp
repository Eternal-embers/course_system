#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define COURSESINFO_PATH "data\\courses_info.txt"   //�γ���Ϣ�ļ�·��

//�γ���Ϣ
typedef struct Course
{
	char id[9];//�γ̴���
	char name[50];//�γ�����
	char property[7];//�γ����ʣ�������/ѡ�޿�/���޿�
	float total_time;//��ѧʱ����λ��ʱ
	float exp_time;//ʵ��ѧʱ����λ��ʱ
	char semester[10];//����ѧ��,���һ��
	float credit;//ѧ��
	struct Course* next;//ָ��ָ����һ���γ�
}Course;


Course* courses;//�γ���Ϣ��
int course_cnt;//�γ���Ŀ
char *courses_path = NULL;//�γ���Ϣ�ļ��ľ���·��

/*	==================== �ȴ��û����� ==================== */
void wait(){
	printf("\n");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -\n");
	system("pause>nul");
}

/*	==================== �ͷ������еĿγ���Ϣ ==================== */
void free_courses(Course* head){
	Course* pt;
	while(head){
		pt = head;
		head = head->next;
		free(pt);
	}
}

void print_head(){
	printf("\t\t%15s%50s%10s%10s%10s%10s%10s\n\n","�γ̴���","�γ�����","�γ�����","��ѧʱ","ʵ��ѧʱ","����ѧ��","ѧ��");
}

void print_item(Course *item){
	if(item == NULL) return;
	printf("\t\t%15s%50s%10s%10.1f%10.1f%10s%10.1f\n\n", item->id, item->name, item->property, item->total_time, item->exp_time, item->semester, item->credit);
}

/*	==================== ��ӡ�γ���Ϣ ==================== */
void print_course(Course *head)
{
	if(head == NULL){
		printf("\t\t[Info] ����Ϣ�ɴ�ӡ!\n");
		wait();
		return;
	}

	//��ӡ�����еĿγ���Ϣ
	Course *p = head;
	printf("\t\t  [Info] �ܹ� %d ���γ���Ϣ��\n\n", course_cnt);
	print_head();
	while(p){
		print_item(p);
		p = p->next;
	}

	wait();
}

int scanf_item(Course* item){
	printf("\t\t");
	scanf("%s", item->id);
	if(item->id[0] == '#') return 0;//��������
	scanf("%s%s%f%f%s%f", item->name, item->property, &item->total_time, &item->exp_time, item->semester, &item->credit);
	return 1;
}

/*	==================== ��ӿγ���Ϣ ==================== */
Course *add_course()
{

	Course* head = NULL;//����ͷ�ڵ�

	printf("\t\t�������#�ٻس�����¼�룺\n\n");
	printf("\t\t���\t\t�γ�����\t\t�γ�����\t\t��ѧʱ\t\tʵ��ѧʱ\t\t����ѧ��\t\tѧ��\n");

	Course* pre = NULL;
	Course* cur = NULL;
	int n = 0;
	cur = (Course *)malloc(sizeof(Course));

	int flag = scanf_item(cur);
	if(flag == 0) return NULL;
	char ch;
	while((ch = getchar()) != '\n');//��ջ�����

	while(flag)
	{
	 	n++;
	 	if(n == 1)head = cur;
	 	else pre->next = cur;
		pre = cur;
		cur = (Course *)malloc(sizeof(Course));

        //¼��γ�
		flag = scanf_item(cur);
		while((ch = getchar()) != '\n');//��ջ�����
	}
	pre->next = NULL;

	wait();

	return head;//���ؿγ������ͷ�ڵ�
}

/*	==================== ����γ���Ϣ ==================== */
void save_courses(Course *head)
{
	if(head == NULL){
		printf("\t\t[Info] ����Ϣ�ɱ��棡\n");
        wait();
		return;
	}

	FILE *fp = NULL;
	if( ( fp = fopen(COURSESINFO_PATH,"a+") ) == NULL ){//׷�Ӹ���ģʽ
		printf("\t\t[Error] open file %s error\n", courses_path);
		return;
	}

	//���γ���Ϣд�뵽�ļ���
	printf("\t\tд��:\n");
	Course *p = head;
	while(p){
		fprintf(fp,"\n%-12s%-45s%-12s%-8.1f%-8.1f%-12s%-.1f", p->id, p->name, p->property, p->total_time, p->exp_time, p->semester, p->credit);
		print_item(p);
		p = p->next;
	}

	// ˢ�»��������ر��ļ�
    fflush(fp);
    if (fclose(fp) == EOF)
        printf("[Error] Error closing file %s\n", courses_path);


	//�ͷſռ�
 	free_courses(head);

	wait();
}

/*	==================== ���ļ��ж�ȡ�γ���Ϣ ==================== */
Course *load_courses()
{
	Course* head = NULL;//����ͷ�ڵ�

	FILE *fp = NULL;
	if( ( fp = fopen(COURSESINFO_PATH,"r") )==NULL ){
		printf("\t\t[Error] Open file '%s' error\n", COURSESINFO_PATH);
		printf("\t\t[Error] Please close the program and check whether the course information file '%s' exists!\n", COURSESINFO_PATH);
		return NULL;
	}

	//���ļ��ж�ȡ�γ���Ϣ��������
  	Course* pre = NULL;
	Course* cur = (Course *)malloc(sizeof(Course));
	if (cur == NULL) {// ���malloc�Ƿ�ɹ������ڴ�
    	fprintf(stderr, "Memory allocation failed!\n");
    	fclose(fp);
   		exit(EXIT_FAILURE);
	}

 	fscanf(fp,"%s%s%s%f%f%s%f", cur->id, cur->name, cur->property, &cur->total_time, &cur->exp_time, cur->semester, &cur->credit);
	int i = 1;
	while(!feof(fp)){
		if(i == 1) head = cur;
		else pre->next = cur;
		pre = cur;

		cur = (Course *)malloc(sizeof(Course));
		if (cur == NULL) {// ���malloc�Ƿ�ɹ������ڴ�
    		fprintf(stderr, "Memory allocation failed!\n");
    		fclose(fp);
   			exit(EXIT_FAILURE);
		}
		fscanf(fp,"%s%s%s%f%f%s%f",cur->id, cur->name, cur->property, &cur->total_time, &cur->exp_time, cur->semester, &cur->credit);

		i++;
	}
	pre->next = NULL;
	/* �˴�����Ϊi - 1������ search_by_credit() �б��������һ��Ԫ��ʱ���� */
 	course_cnt = i - 1;//��¼�γ���Ŀ

  	fflush(fp);
 	if (fclose(fp) == EOF)
        printf("[Error] Error closing file %s\n", courses_path);

	return head;
}

/*	==================== ���¼��ؿγ���Ϣ ==================== */
void reload_courses(Course** courses) {//�ı�coursesָ����Ҫ����ָ��
    if (courses == NULL) {
        printf("Error: courses double pointer is NULL.\n");
        return;
    }

	// �ͷžɵĿγ��б�
    free_courses(*courses);

    // ��ȡ�µĿγ��б�������coursesָ��
    *courses = load_courses(); // �µĿγ��б�ֱ�Ӹ�ֵ��*courses

    printf("\t\t[Info] �ܹ����� %d ���γ���Ϣ��\n", course_cnt);
    wait();
}

/*	==================== ���ݿγ����ʲ鿴�γ���Ϣ ==================== */
void search_by_prop(Course* courses){
	//����γ���Ϣ��û�����룬���½�������
	if(courses == NULL) {
		printf("\t\t[Info] �γ���Ϣδ���룬�����½��пγ���Ϣ���롣\n");
		courses = load_courses();
	}

 	printf("\t\t��ѡ��γ�����(��ѡ0/ѡ��1):");
 	int option = 1;
 	scanf("%d", &option);
 	while(option != 0 && option != 1){
        printf("\t\t������0��1!\n");
        printf("\t\t��ѡ��γ�����(��ѡ0/ѡ��1):");
		scanf("%d", &option);
	}

	char property[2][5] = {"��ѡ","ѡ��"};
	Course* p = courses;
	print_head();
	while(p){
		if(strcmp(p->property ,property[option]) == 0) {
			print_item(p);
		}
		p = p->next;
	}
	
	wait();
}

/*	==================== �Կγ̰�ѧ�ֽ��еݼ����� ==================== */
void search_by_credit(Course *courses){
 	//����γ���Ϣ��û�����룬���½�������
	if(courses == NULL) {
		printf("\t\t[Info] �γ���Ϣδ���룬�����½��пγ���Ϣ���롣\n");
		courses = load_courses();
	}

	int n = course_cnt;
	int i, j;

	Course *p[course_cnt + 5];//ָ�����飬��˳��ִ�������е�ÿһ��Ԫ��
	Course *pt = courses;
	for(i = 0; pt; i++){
		p[i] = pt;
		pt = pt->next;
	}

	//��ָ��������в�������
	for(i = 1; i < n; i++){
		Course *t = p[i];
		for(j = i - 1; j >= 0; j--)
			if(t->credit > p[j]->credit) p[j + 1] = p[j];
				else break;
		p[j + 1] = t;
	}

	//��ӡ������
	print_head();
	for(i = 0;i < n;i++) print_item(p[i]);

	wait();
}

/*	==================== ���ݿγ����������γ� ==================== */
void search_by_name(Course * courses)
{
	//����γ���Ϣ��û�����룬���½�������
	if(courses == NULL) {
		printf("\t\t[Info] �γ���Ϣδ���룬�����½��пγ���Ϣ���롣\n");
		courses = load_courses();
	}

	printf("\t\t��������������:\n\t\t>>");
	char key[50];
	scanf("%s", key);
	int key_len = strlen(key);

	//��ջ�����
	char ch;
	while( (ch = getchar()) != '\n');

	print_head();

	int flag = 0;
	Course *p = courses;
	while(p){
		int i = 0,j = 0;
		int name_len = strlen(p->name);

		//����ƥ��
		while(i < key_len && j < name_len){
			if(key[i] == p->name[j]){
				i++;
				j++;
			} else {
				j = j - i + 1;
				i = 0;
			}
		}

		if(i == key_len){
			print_item(p);
			flag = 1;
		}

		p = p->next;
	}

	if(flag == 0) printf("\t\tδ���������!");

	wait();
}

/*	==================== ����ѧ�������γ� ==================== */
void search_by_semester(Course* courses)
{
 //����γ���Ϣ��û�����룬���½�������
	if(courses == NULL) {
		printf("\t\t[Info] �γ���Ϣδ���룬�����½��пγ���Ϣ���롣\n");
		courses = load_courses();
	}

	printf("\t\t������ѧ��(���һ�ϡ���һ�ȵ�):\n\t\t>>");
	char key[50];
	scanf("%s", key);
	int key_len = strlen(key);

	//��ջ�����
	char ch;
	while( (ch = getchar()) != '\n');

	print_head();

	int flag = 0;
	Course *p = courses;
	while(p){
		int i = 0,j = 0;
		int name_len = strlen(p->semester);

		//����ƥ��
		while(i < key_len && j < name_len){
			if(key[i] == p->semester[j]){
				i++;
				j++;
			} else {
				j = j - i + 1;
				i = 0;
			}
		}

		if(i == key_len){
			print_item(p);
			flag = 1;
		}

		p = p->next;
	}

	if(flag == 0) printf("\t\tδ���������!");

	wait();
}

/*	==================== ��ʼ�� ==================== */
char* init(){
	system("mode con cols=200 lines=1000");
 	courses = load_courses();//����γ���Ϣ

    char cwd[1024];//��ǰ�����ռ���ļ�·��
    char course_path[1024];//�γ���Ϣ���ļ�·��
    char* path = NULL;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\t\tCurrent working directory: %s\n", cwd);

		strcpy(course_path, cwd);
  		strcat(course_path, "\\");
  		strcat(course_path, COURSESINFO_PATH);

		path = (char *)malloc(strlen(course_path));
    } else {
        perror("\t\tError getting current working directory\n");
    }

	printf("\t\tLoad courses information from file: %s\n", course_path);
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -\n");

	return path;
}

/*	==================== �˵� ==================== */
void menu(){
	printf("\n\n");
	printf("\t\t________________________________________________________________________\n") ;
	printf("\t\t|                                                                      |\n");
	printf("\t\t| > > > > > > > > > > > > > ѧ���γ̹���ϵͳ < < < < < < < < < < < < < |\n");
	printf("\t\t|______________________________________________________________________|\n\n") ;
	printf("\t\t                  �˵�>>>\n");
	printf("\t\t                  0 - - - - - -�˳�\n\n");
	printf("\t\t                  1 - - - - - -�鿴���пγ���Ϣ\n\n");
	printf("\t\t                  2 - - - - - -���γ����ʲ鿴\n\n");
	printf("\t\t                  3 - - - - - -��ѧ�ֲ鿴\n\n");
	printf("\t\t                  4 - - - - - -�����γ�\n\n");
	printf("\t\t                  5 - - - - - -��ѧ�ڲ�ѯ\n\n");
	printf("\t\t                  6 - - - - - -¼��γ�\n\n");
	printf("\t\t                  7 - - - - - -��������γ�\n\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -\n");
	printf("\t\t>>");
}

int main()
{
	courses_path = init();//��ȡ�γ���Ϣ�ļ��ľ���·��

	int option = -1;
	while(1){
		menu();
		scanf("%d", &option);

		switch(option){
			case 0: {
				printf("\n\t\t>>> �������˳� <<<");
				return 0;
			}
			case 1:{
				print_course(courses);
				break;
			}

			case 2:{
				search_by_prop(courses);
				break;
			}

			case 3:{
				search_by_credit(courses);
				break;
			}

			case 4:{
				search_by_name(courses);
				break;
			}

			case 5:{
				search_by_semester(courses);
				break;
			}

			case 6:{
				Course* new_course = add_course();
				save_courses(new_course);
				break;
			}

			case 7:{
				reload_courses(&courses);
				break;
			}

			default:{
				printf("\t\t����ѡ��������������롣\n");
				break;
			}
		}
	}

}

