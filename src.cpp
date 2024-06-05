#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define COURSESINFO_PATH "data\\courses_info.txt"   //课程信息文件路径

//课程信息
typedef struct Course
{
	char id[9];//课程代码
	char name[50];//课程名称
	char property[7];//课程性质，公开课/选修课/必修课
	float total_time;//总学时，单位课时
	float exp_time;//实验学时，单位课时
	char semester[10];//开课学期,如大一上
	float credit;//学分
	struct Course* next;//指针指向下一个课程
}Course;


Course* courses;//课程信息表
int course_cnt;//课程数目
char *courses_path = NULL;//课程信息文件的绝对路径

/*	==================== 等待用户操作 ==================== */
void wait(){
	printf("\n");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -\n");
	system("pause>nul");
}

/*	==================== 释放链表中的课程信息 ==================== */
void free_courses(Course* head){
	Course* pt;
	while(head){
		pt = head;
		head = head->next;
		free(pt);
	}
}

void print_head(){
	printf("\t\t%15s%50s%10s%10s%10s%10s%10s\n\n","课程代码","课程名称","课程性质","总学时","实验学时","开课学期","学分");
}

void print_item(Course *item){
	if(item == NULL) return;
	printf("\t\t%15s%50s%10s%10.1f%10.1f%10s%10.1f\n\n", item->id, item->name, item->property, item->total_time, item->exp_time, item->semester, item->credit);
}

/*	==================== 打印课程信息 ==================== */
void print_course(Course *head)
{
	if(head == NULL){
		printf("\t\t[Info] 无信息可打印!\n");
		wait();
		return;
	}

	//打印链表中的课程信息
	Course *p = head;
	printf("\t\t  [Info] 总共 %d 条课程信息：\n\n", course_cnt);
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
	if(item->id[0] == '#') return 0;//结束输入
	scanf("%s%s%f%f%s%f", item->name, item->property, &item->total_time, &item->exp_time, item->semester, &item->credit);
	return 1;
}

/*	==================== 添加课程信息 ==================== */
Course *add_course()
{

	Course* head = NULL;//链表头节点

	printf("\t\t编号输入#再回车结束录入：\n\n");
	printf("\t\t编号\t\t课程名称\t\t课程性质\t\t总学时\t\t实验学时\t\t开课学期\t\t学分\n");

	Course* pre = NULL;
	Course* cur = NULL;
	int n = 0;
	cur = (Course *)malloc(sizeof(Course));

	int flag = scanf_item(cur);
	if(flag == 0) return NULL;
	char ch;
	while((ch = getchar()) != '\n');//清空缓冲区

	while(flag)
	{
	 	n++;
	 	if(n == 1)head = cur;
	 	else pre->next = cur;
		pre = cur;
		cur = (Course *)malloc(sizeof(Course));

        //录入课程
		flag = scanf_item(cur);
		while((ch = getchar()) != '\n');//清空缓冲区
	}
	pre->next = NULL;

	wait();

	return head;//返回课程链表的头节点
}

/*	==================== 保存课程信息 ==================== */
void save_courses(Course *head)
{
	if(head == NULL){
		printf("\t\t[Info] 无信息可保存！\n");
        wait();
		return;
	}

	FILE *fp = NULL;
	if( ( fp = fopen(COURSESINFO_PATH,"a+") ) == NULL ){//追加更新模式
		printf("\t\t[Error] open file %s error\n", courses_path);
		return;
	}

	//将课程信息写入到文件中
	printf("\t\t写入:\n");
	Course *p = head;
	while(p){
		fprintf(fp,"\n%-12s%-45s%-12s%-8.1f%-8.1f%-12s%-.1f", p->id, p->name, p->property, p->total_time, p->exp_time, p->semester, p->credit);
		print_item(p);
		p = p->next;
	}

	// 刷新缓冲区并关闭文件
    fflush(fp);
    if (fclose(fp) == EOF)
        printf("[Error] Error closing file %s\n", courses_path);


	//释放空间
 	free_courses(head);

	wait();
}

/*	==================== 从文件中读取课程信息 ==================== */
Course *load_courses()
{
	Course* head = NULL;//链表头节点

	FILE *fp = NULL;
	if( ( fp = fopen(COURSESINFO_PATH,"r") )==NULL ){
		printf("\t\t[Error] Open file '%s' error\n", COURSESINFO_PATH);
		printf("\t\t[Error] Please close the program and check whether the course information file '%s' exists!\n", COURSESINFO_PATH);
		return NULL;
	}

	//从文件中读取课程信息到链表中
  	Course* pre = NULL;
	Course* cur = (Course *)malloc(sizeof(Course));
	if (cur == NULL) {// 检查malloc是否成功分配内存
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
		if (cur == NULL) {// 检查malloc是否成功分配内存
    		fprintf(stderr, "Memory allocation failed!\n");
    		fclose(fp);
   			exit(EXIT_FAILURE);
		}
		fscanf(fp,"%s%s%s%f%f%s%f",cur->id, cur->name, cur->property, &cur->total_time, &cur->exp_time, cur->semester, &cur->credit);

		i++;
	}
	pre->next = NULL;
	/* 此处必须为i - 1，否则 search_by_credit() 中遍历到最后一个元素时出错。 */
 	course_cnt = i - 1;//记录课程数目

  	fflush(fp);
 	if (fclose(fp) == EOF)
        printf("[Error] Error closing file %s\n", courses_path);

	return head;
}

/*	==================== 重新加载课程信息 ==================== */
void reload_courses(Course** courses) {//改变courses指针需要二级指针
    if (courses == NULL) {
        printf("Error: courses double pointer is NULL.\n");
        return;
    }

	// 释放旧的课程列表
    free_courses(*courses);

    // 获取新的课程列表，并更新courses指针
    *courses = load_courses(); // 新的课程列表将直接赋值给*courses

    printf("\t\t[Info] 总共载入 %d 条课程信息。\n", course_cnt);
    wait();
}

/*	==================== 根据课程性质查看课程信息 ==================== */
void search_by_prop(Course* courses){
	//如果课程信息表没有载入，重新进行载入
	if(courses == NULL) {
		printf("\t\t[Info] 课程信息未载入，将重新进行课程信息载入。\n");
		courses = load_courses();
	}

 	printf("\t\t请选择课程性质(必选0/选修1):");
 	int option = 1;
 	scanf("%d", &option);
 	while(option != 0 && option != 1){
        printf("\t\t请输入0或1!\n");
        printf("\t\t请选择课程性质(必选0/选修1):");
		scanf("%d", &option);
	}

	char property[2][5] = {"必选","选修"};
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

/*	==================== 对课程按学分进行递减排序 ==================== */
void search_by_credit(Course *courses){
 	//如果课程信息表没有载入，重新进行载入
	if(courses == NULL) {
		printf("\t\t[Info] 课程信息未载入，将重新进行课程信息载入。\n");
		courses = load_courses();
	}

	int n = course_cnt;
	int i, j;

	Course *p[course_cnt + 5];//指针数组，按顺序执行链表中的每一个元素
	Course *pt = courses;
	for(i = 0; pt; i++){
		p[i] = pt;
		pt = pt->next;
	}

	//对指针数组进行插入排序
	for(i = 1; i < n; i++){
		Course *t = p[i];
		for(j = i - 1; j >= 0; j--)
			if(t->credit > p[j]->credit) p[j + 1] = p[j];
				else break;
		p[j + 1] = t;
	}

	//打印排序结果
	print_head();
	for(i = 0;i < n;i++) print_item(p[i]);

	wait();
}

/*	==================== 根据课程名称搜索课程 ==================== */
void search_by_name(Course * courses)
{
	//如果课程信息表没有载入，重新进行载入
	if(courses == NULL) {
		printf("\t\t[Info] 课程信息未载入，将重新进行课程信息载入。\n");
		courses = load_courses();
	}

	printf("\t\t请输入搜索内容:\n\t\t>>");
	char key[50];
	scanf("%s", key);
	int key_len = strlen(key);

	//清空缓冲区
	char ch;
	while( (ch = getchar()) != '\n');

	print_head();

	int flag = 0;
	Course *p = courses;
	while(p){
		int i = 0,j = 0;
		int name_len = strlen(p->name);

		//暴力匹配
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

	if(flag == 0) printf("\t\t未搜索到结果!");

	wait();
}

/*	==================== 根据学期搜索课程 ==================== */
void search_by_semester(Course* courses)
{
 //如果课程信息表没有载入，重新进行载入
	if(courses == NULL) {
		printf("\t\t[Info] 课程信息未载入，将重新进行课程信息载入。\n");
		courses = load_courses();
	}

	printf("\t\t请输入学期(如大一上、大一等等):\n\t\t>>");
	char key[50];
	scanf("%s", key);
	int key_len = strlen(key);

	//清空缓冲区
	char ch;
	while( (ch = getchar()) != '\n');

	print_head();

	int flag = 0;
	Course *p = courses;
	while(p){
		int i = 0,j = 0;
		int name_len = strlen(p->semester);

		//暴力匹配
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

	if(flag == 0) printf("\t\t未搜索到结果!");

	wait();
}

/*	==================== 初始化 ==================== */
char* init(){
	system("mode con cols=200 lines=1000");
 	courses = load_courses();//载入课程信息

    char cwd[1024];//当前工作空间的文件路径
    char course_path[1024];//课程信息的文件路径
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

/*	==================== 菜单 ==================== */
void menu(){
	printf("\n\n");
	printf("\t\t________________________________________________________________________\n") ;
	printf("\t\t|                                                                      |\n");
	printf("\t\t| > > > > > > > > > > > > > 学生课程管理系统 < < < < < < < < < < < < < |\n");
	printf("\t\t|______________________________________________________________________|\n\n") ;
	printf("\t\t                  菜单>>>\n");
	printf("\t\t                  0 - - - - - -退出\n\n");
	printf("\t\t                  1 - - - - - -查看所有课程信息\n\n");
	printf("\t\t                  2 - - - - - -按课程性质查看\n\n");
	printf("\t\t                  3 - - - - - -按学分查看\n\n");
	printf("\t\t                  4 - - - - - -搜索课程\n\n");
	printf("\t\t                  5 - - - - - -按学期查询\n\n");
	printf("\t\t                  6 - - - - - -录入课程\n\n");
	printf("\t\t                  7 - - - - - -重新载入课程\n\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -");
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - -\n");
	printf("\t\t>>");
}

int main()
{
	courses_path = init();//获取课程信息文件的绝对路径

	int option = -1;
	while(1){
		menu();
		scanf("%d", &option);

		switch(option){
			case 0: {
				printf("\n\t\t>>> 程序已退出 <<<");
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
				printf("\t\t输入选项错误！请重新输入。\n");
				break;
			}
		}
	}

}

