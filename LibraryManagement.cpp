using namespace std;
#include <iostream>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <string>
#define rsum 10
class Date{
private:
	int year,month,day;
public:
	Date();
	int getYear(){ return year;}
	int getMonth(){ return month;}
	int getDay(){ return day;}
	void setYear(int y){ year = y;}
	void setMonth(int m){ month = m;}
	void setDay(int d){ day = d;}
	int operator-(Date d);
	int numOfDay(int month);
	bool isLeapyear(int year);
	int days();
};
Date::Date(){
	SYSTEMTIME time;
    GetSystemTime(&time);
    year=time.wYear;
    month=time.wMonth;
    day=time.wDay;
}

bool Date::isLeapyear(int year){
	if(((year%4==0)&&(year%100!=0))||(year%400==0))
		return true;
	else
		return false;
}
int Date::numOfDay(int month){
	if((month==1)||(month==3)||(month==5)||(month==7)||(month==8)||(month==10)||(month==12))
		return 31;
	else{
		if(month==2)
			return (28+isLeapyear(year));
		else
			return 30;
	}
}
int Date::days(){
	int i=0,j=0,sum=0;
	for(i=1;i<year;i++)
		sum=sum+(365+isLeapyear(i));
	for(j=1;j<month;j++)
		sum=sum+numOfDay(j);
	sum=sum+day;
	return sum;
}

int Date::operator-(Date d){
	int sum1 = days();
	int sum2 = d.days();
	return (sum2 - sum1)>0?sum2-sum1:sum1-sum2;
}
class Book{
private:
	long bookNumber;
	char name[32];
	int amount;
	Date d;
public:
	Book(){
		bookNumber = -1;
		strcpy(name,"EOF");
		amount = 0;
	}
	Book(long b,char n[],int a);
	Book(Book& b);
	long getBookNumber(){ return bookNumber;}
	char* getName(){ return name;}
	int getAmount(){ return amount;}
	void setBookNumber(long b){ bookNumber=b;}
	void setName(char n[]){
		strcpy(name,n);
	}
	void setAmount(int a){ amount+=a;}
	bool equals(Book b);
	void print(){
		cout<<"书编号：    "<<bookNumber<<endl<<endl;
		cout<<"书名：      "<<name<<endl<<endl;
		cout<<"数量：      "<<amount<<endl<<endl;
	}
	void setDate(){ d = *(new Date());}
	Date* getDate(){ return &d;}
};
Book::Book(long b,char n[],int a){
	bookNumber = b;
	strcpy(name,n);
	amount = a;
}
Book::Book(Book& b){
	if(this!=&b){
		bookNumber = b.bookNumber;
	    strcpy(name,b.name);
	    amount = b.getAmount();
	}
}
bool Book::equals(Book b){
	if(bookNumber==b.getBookNumber())
		return true;
	else
		return false;
}

class BookNode{
private:
	Book* bk;
	BookNode* next;
	Date* dt;
public:
	BookNode(Book* b=NULL);
	BookNode(BookNode& bn);
	Book* getBook(){ return bk;}
	BookNode* getNext(){ return next;}
	Date* getDate(){ return dt;}
	void setBook(Book* b){
		bk = new Book(*b);
	}
	void setNext(){
		next=NULL;
	}
	void setNext(Book* b){
		if(bk!=NULL)
			next = new BookNode(b);
		else{
			bk = b;
			next = NULL;
		}
	}
	void setDate(){
		dt = new Date();
	}
};
BookNode::BookNode(Book* b){
	if(b!=NULL)
		bk = new Book(*b);
	else
		bk=NULL;
    next = NULL;
	dt=NULL;
}

class BookQueue{
private:
	BookNode* head;
public:
	BookQueue(){ head=NULL;}
	BookQueue(BookQueue& q);
	void add(Book* b);
	void add(Book* b,bool flag);
	Book* decrease(Book* b);
	int search(Book* b);
	BookNode* getHead(){ return head;}
};
BookQueue::BookQueue(BookQueue& q){
	if(this==&q){
		head = NULL;
	    BookNode* tmp1 = q.getHead();
	    head = tmp1;
	    BookNode* tmp2 = head;
        while(tmp1->getNext()!=NULL){
			BookNode* tmp1 = new BookNode((tmp1->getBook()));
		    tmp2->setNext(tmp1->getBook());
		}
	}
}
void BookQueue::add(Book* b){
	BookNode* tmp1=head;
	BookNode* tmp2 = new BookNode(b);
	if(head==NULL){
		head=tmp2;
		return;
	}

	while(tmp1->getNext()!=NULL)
		tmp1=tmp1->getNext();
	tmp1->setNext(tmp2->getBook());
	return;
}
void BookQueue::add(Book* b,bool flag){
	BookNode* tmp2 = new BookNode(b);
	BookNode* tmp1;
	if(flag)
		tmp2->setDate();
	if(head==NULL){
		head=tmp2;
		return;
	}
	else
		tmp1=head;

	while(tmp1->getNext()!=NULL)
		tmp1=tmp1->getNext();
	tmp1->setNext(tmp2->getBook());
}
int BookQueue::search(Book* b){
	BookNode* tmp = head;
	int i=-1;
	while(tmp!=NULL){
		++i;
		if((tmp->getBook())->equals(*b))
			return i;
		tmp=tmp->getNext();
	}
	return -1;
}

Book* BookQueue::decrease(Book* b){
	BookNode* tmp=head;
	int i = 0;
	if((i=search(b))!=-1){
		if(tmp->getNext()==NULL){
			head=NULL;
			return tmp->getBook();
		}
		for(int j=0;j<i-1;j++)
			tmp=tmp->getNext();
		if(((tmp->getNext())->getNext())!=NULL)
			tmp->setNext((tmp->getNext()->getNext())->getBook());
		else
			tmp->setNext();
		return b;
	}
	else
		return NULL;
}
class BookFile{
private:
	char* fileName;
	BookQueue* list;
public:
	BookFile(char* fn = "test.dat");
	BookQueue* getBookQueue(){ return list;}
	BookQueue* readBookQueue();
	void writeBook(Book* db);
	void writeBookQueue();
};
BookFile::BookFile(char* fn){
	fileName = fn;
	list = new BookQueue();
}
BookQueue* BookFile::readBookQueue(){
	ifstream inFile;
	inFile.open(fileName);
	if(inFile){
		Book* b = new Book();
	    while(true){
			inFile.read((char*)b,sizeof(*b));
			if(b->getName()!=NULL)
				break;
            list->add(b);
		    if(!inFile.eof())
				break;
		}
	}
	return list;
}
void BookFile::writeBookQueue(){
	ofstream outFile;
	outFile.open(fileName);
	Book* b;
	BookNode* bn = list->getHead();
	while(bn!=NULL){
		b = bn->getBook();
	    outFile.write((char*)b,sizeof(*b));
		if(!outFile)
			break;
		bn = bn->getNext();
	}
}
void BookFile::writeBook(Book* b){
	list->add(b);
}

class Reader{
protected:
	char name[32];
	char academe[32];
	long libraryCardNumber;
	Book bookQueue[rsum];
	int top;
public:
	Reader(){
		strcpy(name,"EOF");
		strcpy(academe,"EOF");
		libraryCardNumber=0;
		for(int i = 0;i<10;i++)
			 bookQueue[i] = *(new Book());
	}

    Reader(char n[],char a[],long l);
	Reader(Reader& dr){
		strcpy(name,dr.name);
		strcpy(academe,dr.academe);
		libraryCardNumber = dr.libraryCardNumber;
	}
	virtual bool overDue(Book* b){ return false; }
	char* getName(){ return name;}
	char* getAcademe(){ return academe;}
	long getLibraryCardNumber(){ return libraryCardNumber;}
	int getTop(){ return top;}
    void setName(char n[]){
		strcpy(name,n);
	}
	void setAcademe(char a[]){
		strcpy(academe,a);
	}
    Book* getBookQueue(){ return bookQueue;}
	int canBorrow(Book* b);
	bool borrow(Book* b);
	bool turnBack(Book* b);
	void setLibraryCardNumber(long l){ libraryCardNumber=l;}
	bool equals(Reader* r);
	void print(){
		cout<<"名字： "<<name<<endl<<endl;
		cout<<"学院： "<<academe<<endl<<endl;
		cout<<"图书证： "<<libraryCardNumber<<endl<<endl;
	}
	bool search(Book* b){
		int j =0;
		for(int i =0;i<rsum;i++){
			while(strcmp(bookQueue[j].getName(),"EOF")!=0)
				j++;
			if(bookQueue[j].equals(*b))
				return true;
		}
		return false;
	}
};
Reader::Reader(char n[],char a[],long l){
		 strcpy(name,n);
		 strcpy(academe,a);
         libraryCardNumber = l;
		 top = 0;
		 for(int i = 0;i<10;i++)
			 bookQueue[i] = *(new Book());
	};
int Reader::canBorrow(Book* b){
	int i=0;
	BookQueue* bq = (new BookFile())->readBookQueue();
	if((i=bq->search(b))!=-1)
		return i;
	else
		return -1;
}
bool Reader::borrow(Book* b){
	int i = 0,j=0;
	BookQueue* bq = (new BookFile())->readBookQueue();
	BookNode* bn;
	if((i=canBorrow(b))!=-1){
		bn = bq->getHead();
		for(int z = 0;z<i;i++)
			bn = bn->getNext();
		if(bn->getBook()->getAmount()>0){
			if(top>10){
				cout<<"over borrow"<<endl;
				return false;
			}
			while(strcmp(bookQueue[j].getName(),"EOF")!=0)
				j++;
			bookQueue[j] = *(bn->getBook());
			bookQueue[j].setDate();
			bn->getBook()->setAmount(-1);
			top++;
		}
		return true;
	}
	return false;
}
bool Reader::turnBack(Book* b){
	int i = 0;
	BookQueue* bq = (new BookFile())->readBookQueue();
	BookNode* bn;
	if((i=canBorrow(b))!=-1){
		bn = bq->getHead();
		for(int j = 0;j<i;i++)
			bn = bn->getNext();
		for(int k = 0;k<top;k++){
			if(bookQueue[k].equals(*b)){
				bookQueue[k] = *(new Book());
				bn->getBook()->setAmount(1);
				top--;
				return true;
			}
		}
	}
	return false;
}
bool Reader::equals(Reader* r){
	if(libraryCardNumber==r->getLibraryCardNumber())
		return true;
	else
		return false;
}
class Teacher:public Reader{
private:
	long jobNumber;
public:
	Teacher(char n[],char a[],long l,long j):Reader(n,a,l),jobNumber(j){}
	long getJobNumber(){ return jobNumber;}
	void setJobNumber(long j){ jobNumber=j;}
};
class Professor:public Teacher{
private:
	char* professionalTitle;
public:
	Professor(char n[],char a[],long l,long j):Teacher(n,a,l,j){
		professionalTitle="professor";
	}
	bool overDue(Book* b);
};
bool Professor::overDue(Book* b){
	int i = 0;
	Date* d = new Date();
	for(int j = 0;j<top;j++){
		if(bookQueue[j].getBookNumber()==b->getBookNumber()){
			if((*(bookQueue[j].getDate())-(*d))>180)
				return true;
			else
				return false;
		}
	}
	cout<<"未找到书！！！"<<endl;
	return false;
}
class Others:public Teacher{
private:
	char* professionalTitle;
public:
	Others(char n[],char a[],long l,long j):Teacher(n,a,l,j){
		professionalTitle="others";
	}
	bool overDue(Book* b);
};
bool Others::overDue(Book* b){
	int i = 0;
	Date* d = new Date();
	for(int j = 0;j<top;j++){
		if(bookQueue[j].getBookNumber()==b->getBookNumber()){
			if((*(bookQueue[j].getDate())-(*d))>90)
				return true;
			else
				return false;
		}
	}
	cout<<"未找到书！！！"<<endl;
	return false;
}
class Student:public Reader{
private:
	long studentID;
public:
	Student(char n[],char a[],long l,long s):Reader(n,a,l),studentID(s){}
	long getStudentID(){ return studentID;}
	long SetStudentID(long s){ studentID=s;}
	bool overDue(Book* b);
};
bool Student::overDue(Book* b){
	int i = 0;
	Date* d = new Date();
	for(int j = 0;j<top;j++){
		if(bookQueue[j].getBookNumber()==b->getBookNumber()){
			if((*(bookQueue[j].getDate())-*d)>60)
				return true;
			else
				return false;
		}
	}
	cout<<"未找到书！！！"<<endl;
	return false;
}
class ReaderNode{
private:
	Reader* rd;
	ReaderNode* next;
public:
	ReaderNode(Reader* r=NULL);
	Reader* getReader(){ return rd;}
	ReaderNode* getNext(){ return next;}//返回当前
	void setReader(Reader* b){
		rd = new Reader(*b);
	}
	void setNext(){
		next=NULL;
	}
	void setNext(Reader* b){
		if(rd!=NULL)
			next = new ReaderNode(b);
		else{
			rd=b;
			next=NULL;
		}
	}
};
ReaderNode::ReaderNode(Reader* r){
	if(r!=NULL)
		rd = new Reader(*r);
	else
		rd = NULL;
	next = NULL;
}
class ReaderQueue{
private:
	ReaderNode* list;
	int capacity;
public:
	static int amount;
	ReaderQueue(int cap = 100);
	static int getAmount(){ return amount;}
	ReaderNode* getList(){ return list;}
	void setList(){ list = NULL;}
    bool add(Reader* r);
	bool decrease(Reader* r);
	int search(long l);
};
int ReaderQueue::amount=0;
ReaderQueue::ReaderQueue(int cap){
	list = new ReaderNode();
	capacity = cap;
}
bool ReaderQueue::add(Reader* r){
	ReaderNode* tmp=list;
	if(list->getReader()==NULL){
		list->setReader(r);
		amount++;
		return true;
	}
	if(search(r->getLibraryCardNumber())==-1){
		while(tmp->getNext()!=NULL)
			tmp=tmp->getNext();
		tmp->setNext(r);
		amount++;
		return true;
	}
	else
		return false;
}
bool ReaderQueue::decrease(Reader* r){
	ReaderNode* tmp=list;
	int i = 0;
	if(tmp!=NULL){
		if((i=search(r->getLibraryCardNumber()))!=-1){
			if(tmp->getNext()==NULL){
				list=NULL;
		        amount--;
				return true;
			}
		for(int j=0;j<i-1;j++)
			tmp=tmp->getNext();
		if(((tmp->getNext())->getNext())!=NULL)
			tmp->setNext((tmp->getNext()->getNext())->getReader());
		else
			tmp->setNext();
		amount--;
		return true;
		}
	}
	return false;
}
int ReaderQueue::search(long l){
	ReaderNode* tmp = list;
	int i=-1;
	while(tmp!=NULL){
		++i;
		if((tmp->getReader()->getLibraryCardNumber())==l)
			return i;
		tmp=tmp->getNext();
	}
	return -1;
}
class LibraryBook{
private:
	BookFile* bf;
public:
	LibraryBook(){ bf = new BookFile();}
	BookFile* getBookFile(){ return bf;}
};
class Administrator{
private:
	LibraryBook* lb;
	ReaderQueue* rq;
public:
	Administrator();
	~Administrator();
	bool regist(Reader* r);
	bool logout(long l);
	ReaderQueue* bookBorrows(long l);
	ReaderQueue* overdueReaders();
	int getReaderAmount();
	ReaderQueue* getReaderQueue(){ return rq;}
};
Administrator::Administrator(){
	ifstream inFile;
	inFile.open("test1.dat");
	rq = new ReaderQueue();
	if(inFile){
		Reader* r = new Reader();
		while(true){
			if(inFile.eof()){
				cout<<"end of file"<<endl;
				break;
			}
			inFile.read((char*)r,sizeof(*r));
			if(!inFile)
				break;
			rq->add(r);
		}
	}
	else
		cout<<"fail to open"<<endl;
	lb = new LibraryBook();
	lb->getBookFile()->readBookQueue();
	inFile.close();
}
bool Administrator::regist(Reader* r){
	ReaderNode* tmp1=rq->getList();
	if(tmp1->getReader()==NULL){
		tmp1->setReader(r);
		rq->amount++;
		return true;
	}
	if(rq->search(r->getLibraryCardNumber())==-1){
		while(tmp1->getNext()!=NULL)
			tmp1=tmp1->getNext();
		tmp1->setNext(r);
		rq->amount++;
		return true;
	}
	else
		return false;
}
bool Administrator::logout(long l){
	ReaderNode* tmp=rq->getList();
	int i = 0;
	if(tmp->getReader()!=NULL){
		if((i=rq->search(l))!=-1){
			if(tmp->getNext()==NULL){
				rq->setList();
		        rq->amount--;
				return true;
			}
		for(int j=0;j<i-1;j++)
			tmp=tmp->getNext();
		if(((tmp->getNext())->getNext())!=NULL)
			tmp->setNext((tmp->getNext()->getNext())->getReader());
		else
			tmp->setNext();
		rq->amount--;
		return true;
		}
	}
	return false;
}
ReaderQueue* Administrator::bookBorrows(long bn){
	int i = 0;
	ReaderNode* rn_tmp = rq->getList();
	ReaderQueue* rq_tmp = new ReaderQueue();
	Book* b_tmp;
	Reader* r_tmp;
	while(rn_tmp!=NULL){
		if(rn_tmp->getReader()!=NULL)
			r_tmp = rn_tmp->getReader();
		else{
			cout<<"blank ReaderQueue"<<endl;
			return NULL;
		}
		r_tmp = rn_tmp->getReader();
        b_tmp = r_tmp->getBookQueue();
		for(int j =0;j<rsum;j++){
			while(strcmp(b_tmp[i].getName(),"EOF")!=0)
				i++;
			if(b_tmp[i].getBookNumber()==bn){
				rq_tmp->add(r_tmp);
				break;
			}
		}
		rn_tmp = rn_tmp->getNext();
	}
	return rq_tmp;
}
ReaderQueue* Administrator::overdueReaders(){
	int j = 0;
	ReaderNode* rn_tmp = rq->getList();
	ReaderQueue* rq_tmp = new ReaderQueue();
	Book* b_tmp;
	Reader* r_tmp;
	while(rn_tmp!=NULL){
		if(rn_tmp->getReader()!=NULL)
			r_tmp = rn_tmp->getReader();
		else{
			cout<<"blank ReaderQueue"<<endl;
			return NULL;
		}
		b_tmp = r_tmp->getBookQueue();
		for(int i =0;i<rsum;i++){
			while(strcmp(b_tmp[j].getName(),"EOF")!=0)
				j++;
			if(r_tmp->overDue(&(b_tmp[j]))){
				rq_tmp->add(r_tmp);
				break;
			}
		}
		rn_tmp = rn_tmp->getNext();
	}
	if(rq_tmp->getList()->getReader()!=NULL)
		return rq_tmp;
	else
		return NULL;
}

Administrator::~Administrator(){
	ofstream outFile;
	outFile.open("test1.dat");
	Reader* r;
	ReaderNode* rn = rq->getList();
	if(outFile){
		while(rn!=NULL){
			r = rn->getReader();
			outFile.write((char*)r,sizeof(*r));
			if(!outFile)
				break;
		    rn = rn->getNext();
		}
	}
	lb->getBookFile()->writeBookQueue();
}
//管理员登录界面
bool adminLog(){
	char n[32];
	char a[32];
	long l;
	long id;
	int k=0;
	Administrator* admin = new Administrator();
	ReaderQueue* rq = new ReaderQueue();
    ReaderNode* rn = new ReaderNode();
	Reader* r =new Reader();
	long ln;
	printf("*******************管理员登录*******************\n\n");
	printf("用户名：");
//	cin>>address;
	printf("密码：");
//	cin>>password;
r4:	printf("\n\n1.查询图书的借阅情况。\n2.输出当前借阅者数量。\n3.查询超期借阅者。\n4.借阅者注册。\n5.借阅者注销。\n6.返回上一级菜单。\n");
	int i;
	printf("请输入序号：");
	cin>>i;
	switch(i){
	case 1:{
		cout<<"请输入书号：  "<<endl;
		cin>>ln;
		if((rq = admin->bookBorrows(ln))==NULL){
			cout<<"no Readers"<<endl;
			break;
		}
		rn = rq->getList();
		while(rn!=NULL){
			rn->getReader()->print();
			rn = rn->getNext();
		}
		break;
		   }
	case 2:{
		cout<<admin->getReaderQueue()->amount<<endl;
		break;
		   }
	case 3:{
		if((rq = admin->overdueReaders())==NULL){
			cout<<"no readers"<<endl;
		}
        rn = rq->getList();
		while(rn!=NULL){
			rn->getReader()->print();
			rn = rn->getNext();
		}
		break;
		   }
	case 4:{
		cout<<"1.学生 "<<endl;
	    cout<<"2.教授"<<endl;
	    cout<<"3.其它"<<endl;
	    cin>>k;
	    switch(k){
		case 1:{
			cout<<"姓名：  ";
		    cin>>n;
		    cout<<endl;
		    cout<<"学院：   ";
		    cin>>a;
		    cout<<endl;
		    cout<<"图书证号：";
		    cin>>l;
		    cout<<endl;
		    cout<<"学生号:   ";
		    cin>>id;
		    cout<<endl;
		     r = new Student(n,a,l,id);
			 break;
		   }
	case 2:{
		    cout<<"姓名：  ";
		    cin>>n;
		    cout<<endl;
		    cout<<"学院：   ";
		    cin>>a;
		    cout<<endl;
		    cout<<"图书证号：";
		    cin>>l;
		    cout<<endl;
		    cout<<"工作号：  ";
		    cin>>id;
		    cout<<endl;
		    r = new Professor(n,a,l,id);
		    break;
		   }
	case 3:{
		    cout<<"姓名：  ";
		    cin>>n;
		    cout<<endl;
		    cout<<"学院：   ";
		    cin>>a;
		    cout<<endl;
		    cout<<"图书证号：";
		    cin>>l;
		    cout<<endl;
		    cout<<"工作号：   ";
		    cin>>id;
		    cout<<endl;
		    r = new Others(n,a,l,id);
		    break;
		   }
		}
		admin->regist(r);
		break;
		   }
	case 5:	{
		cout<<"请输入图书编号："<<endl;
		cin>>l;
		admin->logout(l);
		break;
		   }
	case 6: {
		delete admin;
		return true;
		break;
			}
	default: printf("您的输入有误，请重新输入！"); goto r4;
	}
	delete admin;
	return false;
}

//借阅者登录界面
bool guestLog(){
	printf("*******************借阅者登录*******************\n");
	printf("用户名：");
//	cin>>address;
	printf("密码：");
//	cin>>password;
r5:	printf("\n\n1.查询图书是否可借。\n2.查询本人的借书状态。\n3.借书。\n4.还书。\n5.返回上一级菜单。\n");
	int i;
	printf("请输入序号：");
	cin>>i;
	switch(i){
	case 1: break;
	case 2:break;
	case 3:break;
	case 4:break;
	case 5:return true;break;
	default: printf("您的输入有误，请重新输入！"); goto r5;
	}
	return false;
}


void main(){
r3:	printf("1.管理员登录。\n2.借阅者登录。\n");
	int i;
	cout<<"请输入序号：";
	cin>>i;
	switch(i){
	case 1:if(adminLog()){
			   cout<<endl;
			   goto r3;
		   }
		   else
			   break;
	case 2:if(guestLog()){
			   cout<<endl;
			   goto r3;
		   }
		   else
			   break;
	default: printf("您的输入有误，请重新输入！");goto r3;
	}
}





