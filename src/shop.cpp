#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;

typedef struct item{               
    string barcode;					//商品条码
    string itemname;				//商品名称
    string unit;					//商品单位，如瓶或个
    string category;				//商品大类别
    string subCategory;				//商品小类别
    string price;					//商品价格
    struct item *item_next;
}item;

typedef struct sub_category{		
    string discountcode;			//打折商品条码
    string subcategoryname;			//打折商品名称
    string discount;				//可以定义打几折
    string discount_min;			//可以定义需要至少买多少商品才够打折
    struct sub_category *sub_next;
}sub_category;

typedef struct checkitem{
    string checkcode;				//结账商品条码
    string checkname;				//结账商品名称
    double checkunit;				//结账商品数量，定义为支持小数的类型，配合当单位为斤或其他时候的使用
    string unitname;				//结账商品单位
    string checkprice;				//结账商品未打折价格
    double itemdiscount;			//可以定义某种结账商品优惠金额
    double itemsum;					//可以定义结账商品小计
    struct checkitem *check_next;    
}checkitem;

class Citem {

private:							//建立三个链表用以分别存储所有商品信息，打折商品信息和结账商品信息
    item *head;						//item对应所有商品信息
    sub_category *head2;			//sub_category打折商品信息
    checkitem *head3;				//checkitem对应结账商品信息
public:
    Citem(){
        head= new item;
        head->barcode="null";
        head->item_next=NULL;
        head2= new sub_category;
        head2->discountcode="null";
        head2->sub_next=NULL;
        head3= new checkitem;
        head3->checkcode="null";
        head3->checkname="null";
        head3->checkunit=0;
        head3->unitname="null";
        head3->checkprice="null";
        head3->itemdiscount=0;
        head3->itemsum=0;
        head3->check_next=NULL;
        
    }
    ~Citem(){
        item *p=head, *q;
        while(p){
            q=p;
            p=q->item_next;
            delete q;
        }
        sub_category *p2=head2, *q2;
        while(p2){
            q2=p2;
            p2=q2->sub_next;
            delete q2;
        }
        checkitem *p3=head3, *q3;
        while(p3){
            q3=p3;
            p3=q3->check_next;
            delete q3;
        }
    }
    item showitem();
    void enteritem();
    void entersub();
    bool insertitem(const item &item_a);
    bool insertsub(const sub_category &sub_category_a);
    bool insertcheck(const checkitem &checkitem_a) const;
    int check() const;
    item *printitem(const string &item_code) const;
    sub_category *findsubcate(const string &sub_name) const;
    item *searchcode(const string &name);
    string item_unit(string str, double *unit) const;
    void splitcomma(string str,vector<string>& res) const;
    
        
    
};


void Citem::enteritem(){
    item tmp;    
    char itemname[]="itemlist.txt";				//从"itemlist.txt"文件中读入商店中所有商品列表
    string itemtmp;
    ifstream readitem;
    readitem.open(itemname,ios::in);
    int line=0;
    if(readitem.fail()){
        cout<<"Cannot find itemlist.txt."<<endl;
        
    }
    else{
        while(getline(readitem,itemtmp,'\n'))
        {
            line++;
            vector < string > itemline;
            //cout<<line<<"line:"<<tmp<<endl;
            splitcomma(itemtmp, itemline);
            tmp.barcode=itemline[0];
            tmp.itemname=itemline[1];
            tmp.unit=itemline[2];
            tmp.category=itemline[3];
            tmp.subCategory=itemline[4];
            tmp.price=itemline[5];
            insertitem(tmp);
        }
    }
    readitem.close();
}

void Citem::entersub(){
    sub_category tmp;
    char subname[]="subCategory.txt";			//从"subCategory.txt"文件中读入某一子类型打折商品对应条码列表
    string subtmp;
    ifstream readsub;
    readsub.open(subname,ios::in);
    
    if(readsub.fail()){
        cout<<"Cannot find subCategory.txt."<<endl;
        
    }
    else{
        while(getline(readsub,subtmp,'\n'))
        {
            vector < string > subline;
            splitcomma(subtmp, subline);
            for(int i=0; i<subline.size(); i++){
                tmp.discountcode=subline[i];
                insertsub(tmp);
            }
			
        }
    }
    readsub.close();
    
}

bool Citem::insertitem(const item &item_a)		//插入item链表节点
{
    item *newnode, *p=head;
    if(p->item_next==NULL){
        p->item_next=new item (item_a);
        p->item_next->item_next=NULL;
        return true;
    }
    while (p->item_next)
    {
        if(p->item_next->barcode==item_a.barcode)
        {
            cout<<"insert false!\n";
            return false;
        }
        if(p->item_next->barcode>item_a.barcode)
        {
            newnode=new item(item_a);
            newnode->item_next=p->item_next;
            p->item_next=newnode;
            return true;
        }
        p=p->item_next;
    }
    p->item_next=new item(item_a);
    p->item_next->item_next=NULL;
    
    return true;
}

bool Citem::insertsub(const sub_category &sub_category_a)	//插入sub_category链表信息
{
    sub_category *newnode2, *p2=head2;
    if(p2->sub_next==NULL){
        p2->sub_next=new sub_category (sub_category_a);
        p2->sub_next->sub_next=NULL;
        return true;
    }
    while (p2->sub_next)
    {
        if(p2->sub_next->discountcode==sub_category_a.discountcode)
        {
            cout<<"insert false!\n";
            return false;
        }
        if(p2->sub_next->discountcode>sub_category_a.discountcode)
        {
            newnode2=new sub_category(sub_category_a);
            newnode2->sub_next=p2->sub_next;
            p2->sub_next=newnode2;
            return true;
        }
        p2=p2->sub_next;
    }
    p2->sub_next=new sub_category(sub_category_a);
    p2->sub_next->sub_next=NULL;
    
    return true;
}

bool Citem::insertcheck(const checkitem &checkitem_a) const			//结账时对输入商品的条码处理
{																	//如果还未扫入该条码就插入checkitem节点
    checkitem *newnode3, *p3=head3;
    if(p3->check_next==NULL){
        p3->check_next=new checkitem (checkitem_a);
        p3->check_next->check_next=NULL;
        return true;
    }
    while(p3->check_next){
        if(p3->check_next->checkcode==checkitem_a.checkcode){		//已经扫过该条码，就增加对应商品的数量
            double unit=checkitem_a.checkunit;
            p3->check_next->checkunit=p3->check_next->checkunit+unit;
            return true;
        }
        if(p3->check_next->checkcode>checkitem_a.checkcode)
        {
            newnode3=new checkitem(checkitem_a);
            newnode3->check_next=p3->check_next;
            p3->check_next=newnode3;
            return true;
        }
        p3=p3->check_next;
    }
    p3->check_next=new checkitem(checkitem_a);
    p3->check_next->check_next=NULL;
    
    return true;
}

int Citem::check() const{							//进行结账
    string codelist;
    cout<<"输入商品编码，以英文状态下的逗号隔开，回车结束:"<<endl;
    cin>>codelist;
    vector<string> code;
    splitcomma(codelist, code);						//对输入的字符串处理，按逗号分割
    int item_num=code.size();
    double sumprice=0;
    double discountsum=0;
    
    
    for(int i=0; i<item_num;i++)
    {
        double unit=1;
      
        string codebar=code[i];
        string tmp=item_unit(codebar,&unit);		//对输入的字符串处理，提取"-"前的字符作为商品条码，"-"后面存入商品数量信息
        item *p;
        p=printitem(tmp); 
	      
        checkitem checktmp;
        checktmp.checkcode=p->item_next->barcode;
        checktmp.checkname=p->item_next->itemname;
        checktmp.checkunit=unit;
        checktmp.unitname=p->item_next->unit;
        checktmp.checkprice=p->item_next->price;        
        insertcheck(checktmp);
        }     
    
    cout<<"\n*<没钱赚商店>购物清单*\n"<<endl;
    checkitem *checkp;
    for(checkp=head3->check_next; checkp; checkp=checkp->check_next)
    {
        double itemprice;
        double unit=checkp->checkunit;
        cout<<"名称："<<checkp->checkname<<"，数量："<<unit<<checkp->unitname;
        cout<<"，单价："<<checkp->checkprice<<"(元)，";
        string price=checkp->checkprice;
        itemprice=atof(price.c_str());
        sumprice+=itemprice*unit;
        string checkcodep=checkp->checkcode;
        sub_category *p2;
        p2=findsubcate(checkcodep);     
        if((p2!=NULL)&&unit>9)
        {
          
          discountsum+=itemprice*0.05*unit;
          checkp->itemdiscount=itemprice*0.05*unit;
		  printf("小计: %.2lf (元)，优惠：%.2lf (元)\n",itemprice*0.95*unit,itemprice*0.05*unit);
          
        }
        else{
          printf("小计: %.2lf (元)\n",itemprice*unit);
			
        }     
    }
    cout<<"____________________________________________________________\n"<<endl;
    if(discountsum!=0){
    cout<<"批发价出售商品:"<<endl;
    for(checkp=head3->check_next; checkp; checkp=checkp->check_next)
    {   
        if(checkp->itemdiscount>0)
        {
          cout<<"名称："<<checkp->checkname<<"，数量："<<checkp->checkunit<<checkp->unitname<<endl;
            
        }
        
        
    }
    cout<<"____________________________________________________________\n"<<endl;
    }
       
	printf("总计: %.2lf (元) ",sumprice-discountsum);
	if(discountsum!=0) printf("节省: %.2lf (元)",discountsum);
    cout<<"\n____________________________________________________________\n"<<endl;
     system("pause");
	 return 0;
}

string Citem::item_unit(string str, double *unit) const			//处理输入返回商品条码和数量
{	string newobj;
	string itemnum;
	string::size_type index=0;
	*unit=str[0];
	int ind=0;
	double res=1;
	stringstream ss;
 for (index;index!=str.size();++index)
 { if ((str[index]!='-')&&ind==0)
	{newobj+=str[index];
	}
	if(str[index]=='-'){ 
	ind=1;
	continue;}
	if((str[index]!='-')&&ind==1)
	{
	itemnum+=str[index];
	}	
 }
 ss<<itemnum;
 ss>>res;
 *unit=res;
 return newobj;
}

void Citem::splitcomma(string str,vector<string>& res) const	//将字符串按照逗号分割
{  
    size_t ind = 0;
    size_t indcomma=str.find_first_of(',',ind);
    while (indcomma!=std::string::npos)
    {
        res.push_back(str.substr(ind,indcomma-ind));
        ind=indcomma+1;
        indcomma=str.find_first_of(',',ind);
    }
    if (indcomma-ind>0)
    {
        res.push_back(str.substr(ind,indcomma-ind));
    }
}

item *Citem::printitem(const string &item_code) const {
    item *p;
    for(p=head;p->item_next;p=p->item_next){
        if(p->item_next->barcode==item_code) return p;        
    }
    return NULL;
}

sub_category *Citem::findsubcate(const string &sub_code) const {
    sub_category *p2;
    for(p2=head2;p2->sub_next;p2=p2->sub_next){
        if(p2->sub_next->discountcode==sub_code) return p2;
    }
    return NULL;
}

int main(){
    Citem a;				//主程序				
    
    a.enteritem();			//读取所有商品列表
    a.entersub();			//读取打折商品条码信息
    a.check();				//进行结账
}