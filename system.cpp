#include <iostream>
#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
using namespace std;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
      ((std::string*)userp)->append((char*)contents, size * nmemb);
          return size * nmemb;}

float P_R_O_F_I_T;

//What you can do :
//Menu +
//Add +
//Remove + 
//Best price + 
//Profit +

//BUGS:
//Doesnt work with a smaller window -

void hmmtm(){
  ofstream write;
  write.open("profit.txt");
  cout<<endl;
  float nr;
  cout<<"How much money do you want to make : $";cin>>nr;
  P_R_O_F_I_T = nr;
  write<<P_R_O_F_I_T<<endl;
  write.close();
}

vector<string> name;
int menu(){
  system("clear");int n;
  cout<<endl;
  cout<<"     1-->Exit"<<endl;
  cout<<"     2-->Add hash"<<endl;
  cout<<"     3-->Refresh"<<endl;
  cout<<"     4-->Show all items"<<endl;
  cout<<"     5-->Best price to sell"<<endl;
  cout<<"     6-->Profit"<<endl;
  cout<<endl;
  cout<<"  Introduceti : ";cin>>n;
  return n;
}

void refres(){
  ofstream erase;
  erase.open("data.txt");
  erase.close();
  ifstream read;
  read.open("add.txt");
  string temp;
  vector<string> hash;
  name.clear();
  int j = 0;
  while(getline(read,temp)){
    if(j==1){
      name.push_back(temp);j=0;
    }else{
    j++;
    hash.push_back(temp);}}
    read.close();
  for(int i = 0;i<hash.size();i++){
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();
  const char* c = hash[i].c_str();
  if(curl) {
    curl_easy_setopt(curl,CURLOPT_URL,c);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    }
  Json::Reader reader;
  Json::Value obj;
  reader.parse(readBuffer,obj); 
  ofstream in;
  in.open("data.txt",ios_base::app);
  in<<obj["lowest_price"].asString()<<endl;
  in<<name[i]<<endl;
  in.close();
  }
}

void removed(){
  ifstream read;
  ofstream write;
  read.open("data.txt");
  write.open("temp.txt");
  string temp;
  int i = 0;
  while(getline(read,temp)){
    if(i==0){
      i++;
      char cstr[temp.size() + 1];
      strcpy(cstr,temp.c_str());
      for(int i = 0;i<temp.size()+1;i++){
        if(cstr[i]=='$'){
          cstr[i]=' ';}
      }
      write<<cstr<<endl;
    }else if(i!=0){
      write<<temp<<endl;
      i=0;
    }
  }
  read.close();
  write.close();
  system("rm data.txt");
  rename("temp.txt","data.txt");
}
void add(){
  system("vi add.txt");
}
void show(){
  system("clear");
  cout<<endl;
  ifstream read;
  read.open("data.txt");
  string temp;int j = 0;
  while(getline(read,temp)){
    if(j==0){
      cout<<"     ";
      cout<<"$";
      cout<<temp<<" ";j++;
    }else if(j!=0){
      cout<<temp<<endl;j=0;
    }
  }
}

void profitlive(){
  system("clear");
  ifstream read;
  read.open("data.txt");
  ifstream pread;
  pread.open("pyp.txt");
  ifstream rwrite;
  rwrite.open("oldprice.txt");

  float ntemp;vector<float> priceud;
  while(rwrite>>ntemp){//We get the old price
    priceud.push_back(ntemp);}

  ofstream write;
  write.open("oldprice.txt");
  vector<float> pyp;float ptemp;
  while(pread>>ptemp){//We get the new number with commision already on
    float nr = ptemp + P_R_O_F_I_T;
    float nrtemp = nr*(float)(15/100.0);
    nrtemp+=nr;
    pyp.push_back(nrtemp);}
  string temp;int j = 0;int i = 0;
  bool prf;  
  if(P_R_O_F_I_T==0){prf = false;
    cout<<endl<<" Profit = "<<"AUTO"<<"$"<<endl;}
  else if(P_R_O_F_I_T!=0){prf = true;
    cout<<endl<<" Profit = "<<P_R_O_F_I_T<<"$"<<endl;}
  cout<<endl<<endl;
  cout<<"   1        2        3           4        5"<<endl;
  cout<<endl;

  while(getline(read,temp)){//We show all the numbers
    if(j==0){
      string out = temp;
      out.erase(0,1);
      stringstream lev(out);
      float x = 0;
      lev>>x;float nrt = 0;
      write<<x<<endl;
      if(prf==true){nrt = x-pyp[i]/*-P_R_O_F_I_T;*/;}
      else if(prf==false){nrt = x-pyp[i];}
      if(x>priceud[i]){//COUT " + $0.32 ;
        cout<<" + ";
        cout<<"$"<<x<<"    ";
      }else if(priceud[i]>x){
        cout<<" - ";
        cout<<"$"<<x<<"    ";
      }else if(priceud[i]==x){
        cout<<" = ";
        cout<<"$"<<x<<"    ";}
      float result = pyp[i];
      cout<<"$";
      cout<<fixed<<setprecision(2)<<result<<"    ";
      if(nrt>0){cout<<"+";}
      if(0>nrt){cout<<"-";}
      cout<<" $"<<nrt<<"   ";

      /*Its working but its not workingxd
      if(result>x){
        cout<<" FALSE    ";
      }else if(x>result){
        cout<<"  TRUE     ";}*/

      if(0>nrt){
        cout<<" FALSE    ";}
      else if(nrt>0){
        cout<<"  TRUE     ";}
      j++;i++;}
    else if(j!=0){
      cout<<temp<<endl;
      j=0;}
  }cout<<endl;

  cout<<"   1-->Lowest price"<<endl;
  cout<<"   2-->Price to sell to make profit"<<endl;
  cout<<"   3-->Your profit"<<endl;
  cout<<"   4-->If you sell now chanches to make profit"<<endl;
  cout<<"   5-->Name of the item"<<endl<<endl;
}
/*
void profit(){
  system("clear");
  ifstream read;
  read.open("data.txt");
  ifstream pread;
  pread.open("pyp.txt");
  ifstream rwrite;
  rwrite.open("price.txt");
  float ntemp;vector<float> priceud;
  while(rwrite>>ntemp){//We get the old price
    priceud.push_back(ntemp);}
  ofstream write;
  write.open("price.txt");
  vector<float> pyp;float ptemp;
  while(pread>>ptemp){//We get the new number with commision already on
    write<<ptemp<<endl;
    float nr = ptemp + P_R_O_F_I_T;
    float nrtemp = nr*(float)(15/100.0);
    nrtemp+=nr;
    pyp.push_back(nrtemp);}
  string temp;int j = 0;int i = 0;
  cout<<"Profit = "<<P_R_O_F_I_T<<"$"<<endl;
  cout<<endl<<endl;
  cout<<"   1        2        3        4"<<endl;
  cout<<endl;
  while(getline(read,temp)){//We show all the numbers
    if(j==0){
      string out = temp;
      out.erase(0,1);
      stringstream lev(out);
      float x = 0;
      lev>>x;
      if(x>priceud[i]){
        cout<<" + ";
        cout<<"$"<<x<<"    ";
      }else if(priceud[i]>x){
        cout<<" - ";
        cout<<"$"<<x<<"    ";
      }else if(priceud[i]==x){
        cout<<" = ";
        cout<<"$"<<x<<"    ";}
      float result = pyp[i];
      cout<<"$";
      cout<<fixed<<setprecision(2)<<result<<"   ";
      if(result>x){
        cout<<" FALSE    ";
      }else if(x>result){
        cout<<" TRUE     ";}
      j++;i++;}
    else if(j!=0){
      cout<<temp<<endl;
      j=0;}
  }cout<<endl;
  cout<<"   1-->Lowest price"<<endl;
  cout<<"   2-->Price to sell to make profit"<<endl;
  cout<<"   3-->If you sell now chanches to make profit"<<endl;
  cout<<"   4-->Name of the item"<<endl;getch();
}*/

/*
void chose(){
  system("clear");
  vector<string> names;
  ifstream read;
  read.open("add.txt");
  ofstream write;
  write.open("temp.txt");
  string temp;int j = 0;int i = 0;
  while(getline(read,temp)){
    if(j!=0){write<<i<<" "<<temp<<endl;
      cout<<i;i++;
      cout<<"     ";
      names.push_back(temp);
      cout<<temp<<endl;j=0;
    }else{j++;}
  }read.close();
  ifstream riad;
  riad.open("temp.txt");
  cout<<endl;int nr;
  cout<<"   Insert the number : ";cin>>nr;
  while(getline(
}*/

void back(){
  while(true){
    cout<<endl;
    cout<<"   Enter q to go back or r to refresh : ";char x;
    cin>>x;
    if(x=='q'){break;}
    if(x=='r'){refres();profitlive();}
  }system("clear");
}

int bmenu(){
  initscr();cbreak;noecho();
  int h,w;
  getmaxyx(stdscr,h,w);
  WINDOW * win = newwin(15,w/3,h-40,30);
  box(win,0,0);
  keypad(win,true);
  string choices[6] = {"Exit","Add","Refresh","Show","Profit","Profit value"};
  int choice;
  int hl = 0;
  while(true){
    for(int i = 0;i<6;i++){
      if(i==hl){
        wattron(win,A_REVERSE);}
        mvwprintw(win,i+5,10,choices[i].c_str());
        wattroff(win,A_REVERSE);
    }choice=wgetch(win);
    switch(choice){
      case KEY_UP:
      hl--;if(hl==-1){hl=0;}
      break;
      case KEY_DOWN:
      hl++;if(hl==6){hl=5;}
      break;
      default:
      break;
    }wrefresh(win);
    if(choice==10){
      wprintw(win,"%d",hl);
      break;
    }
  }
  wrefresh(win);
  endwin();
  return hl;
}
int main(){
  cout<<endl;
  cout<<"  ";
  int n = 0;
  system("clear");
  cout<<endl;
  cout<<"    1-->Graphic menu"<<endl;
  cout<<"    2-->Console menu"<<endl;
  cout<<"  Insert : ";cin>>n;
  while(true){
    system("clear");
    int nr;
    if(n==1){nr = bmenu();nr++;}
    if(n==2){nr = menu();}
    if(nr==1){return 0;}
    if(nr==2){add();}
    if(nr==3){refres();removed();}
    if(nr==4){show();back();}
    if(nr==5){profitlive();back();}
    if(nr==6){hmmtm();}
  }
}
