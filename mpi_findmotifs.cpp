// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"
#include <mpi.h>
struct TreeNode
{
    bits_t val;
    unsigned int sum;
    unsigned int height;
};

std::vector<bits_t> findmotifs_worker(const unsigned int n,
                       const unsigned int l,
                       const unsigned int d,
                       const bits_t* input,
                       const unsigned int startbitpos,
                       bits_t start_value)
{


    unsigned int d0 = hamming(input[0], start_value);
    unsigned int tempd = d;
    unsigned int d1= d - d0;
    unsigned int l1 = l - startbitpos;

    std::vector<bits_t> result;
    std::vector<struct TreeNode> ll;
    //create start node
    struct TreeNode *root;
//======================start node into stack===
    struct TreeNode cur;
    cur.val = start_value;
    unsigned sum0 = 0;
    unsigned height0 = 0;
    cur.sum = sum0;
    cur.height = height0;
    root = &cur;
    
    ll.push_back(*root); 

    bits_t a = root->val;
    int i;

    for (i = 1; i < n; i++) {
        unsigned int dist = hamming(a, input[i]);
        if (dist > d) {
            break;
        }
    }
    if (i == n) {
        result.push_back(a);
    }   

   
    while(!ll.empty()){

        while(root->height < l1 && root->sum < d1){

            struct TreeNode left;
            left.height = (root->height) + 1;
            left.sum = (root->sum) + 1;
            left.val = root->val ^ (1<<(left.height + startbitpos)); 
            root = &left;           

            ll.push_back(*root);  

            bits_t a = root->val;
            unsigned int i;
            for (i = 1; i < n; i++) {
                unsigned int dist = hamming(a, input[i]);
                if (dist > d) {
                    break;
                }
            }
            if (i == n) {   
                result.push_back(a);
            }
        
        }

        struct TreeNode temp = ll.back();
        ll.pop_back();
        root = &temp;


        if (root->height < l1 && root->sum < d1)
        {
            struct TreeNode right;
            right.val = root->val;
            right.height = root->height +1;
            right.sum = root->sum;
            root = &right;

            ll.push_back(*root); 
        } 

    }

    return result;
}

void worker_main()
{
//=========================================bcast=========================================
    
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<bits_t> results;


    unsigned int n = 0;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    unsigned int l = 0;
    MPI_Bcast(&l, 1, MPI_INT, 0, MPI_COMM_WORLD);
    unsigned int d = 0;
    MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);
    unsigned int master_depth = 0;
    MPI_Bcast(&master_depth, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<bits_t> input;
    input.resize(n);
    MPI_Bcast(&input[0],n,MPI_UNSIGNED_LONG,0,MPI_COMM_WORLD);

    
    std::vector<bits_t> start_value,buf;
    start_value.resize(2);    
    buf.resize(2);
    MPI_Status stat,stat_4_buf;
    MPI_Request req_4_buf,req4send;


//====================================receive===================================================

    bits_t rankcycle = -1;
    bits_t cyclerecord = 0;
    std::vector<bits_t> temp_solution;
    std::vector<bits_t> temp_results;
    bits_t count = 0;
    bits_t terminate = 0;

    while(true){
        rankcycle = (rankcycle+1)%(size-1);
        cyclerecord++;
        if(rank == rankcycle + 1){
            MPI_Recv(&start_value[0],2,MPI_UNSIGNED_LONG,0,123,MPI_COMM_WORLD,&stat);//r
            count++;

//============prepare results====================

            MPI_Request req4send;            


            if (start_value[0] == 0 ) {
                results.insert(results.begin(), terminate);
                break;    
            }

            temp_results=findmotifs_worker(n,l,d,&input[0],master_depth,start_value[1]);

            while (!temp_results.empty()) {
                results.insert(results.begin(), temp_results.back());
                temp_results.pop_back();
            }

            temp_solution.insert(temp_solution.begin(),start_value[1]);

            bits_t presend_results = results.back();
          
        }
           
    }


    int flag=0;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Request req_send;
    bits_t sendout = 666;

    
    //============   worker  send ===============
    bits_t count_send_worker = 0;
    while (!results.empty()) {
        bits_t temp2send = results.back();
        results.pop_back();
        
        MPI_Request req5send;
        
        MPI_Isend(&temp2send,1,MPI_UNSIGNED_LONG,0,123,MPI_COMM_WORLD, &req5send);//@@@@@@@@@@
        count_send_worker++;
        
    }
  

}


std::vector<bits_t> findmotifs_master(const unsigned int n,
                                      const unsigned int l,
                                      const unsigned int d,
                                      const bits_t* input,
                                      const unsigned int till_depth)
{
    std::vector<bits_t> results;
    return results;
}
//====================rank = 0==================================
std::vector<bits_t> master_main(unsigned int n, unsigned int l, unsigned int d,
                                const bits_t* input, unsigned int master_depth)
{

    std::vector<bits_t> results;
//========================   b  c  a   s    t =============================================================
 
    int rank, p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&l, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&master_depth, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<bits_t> number;

    for ( unsigned int i = 0; i < n; i++)
    {
                number.push_back(input[i]);
    }
    MPI_Bcast(&number[0], n, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);


//==================   make tree   ==============================================
    
bits_t send_count=0;
        std::vector<bits_t> result;
        std::vector<struct TreeNode> ll;
        //create start node
        struct TreeNode *root;
        struct TreeNode cur;
        cur.val = input[0];
        unsigned sum0 = 0;
        unsigned height0 = 0;
        cur.sum = sum0;
        cur.height = height0;
        root = &cur;
        //start node into stack
        ll.push_back(*root); 
        //add to result
        bits_t a = root->val;


//@@@@@=====================   virtual node m a k e   message  =============================================================
                        
        send_count++;
        std::vector<bits_t> messege0;
        std::vector<bits_t> buffer;
        messege0.resize(2);

        bits_t marker=1;
        messege0[0]=marker;
        messege0[1]=a;

        MPI_Status stat;//used to check message source
        MPI_Request req;
        bits_t source;
        bits_t size_count=0;


        MPI_Comm_size(MPI_COMM_WORLD, &p);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
                

 
                
//@@@@@@====================  virtual node   s  e  n   d  =============================================================
                
     
MPI_Isend(&messege0[0],2,MPI_UNSIGNED_LONG,1,123,MPI_COMM_WORLD,&req);

        bits_t rankcycle = -1;
        bits_t cyclerecord = 0;

        while(!ll.empty()){

            while(root->height < master_depth && root->sum < d){  

                struct TreeNode left;
                left.height = (root->height) + 1;
                left.sum = (root->sum) + 1;
                left.val = root->val ^ (1<<(left.height)); 
                root = &left;           

                ll.push_back(*root);  
               
                bits_t a = root->val;


//========================   m a k e   message  =============================================================
                
                send_count++;
                std::vector<bits_t> messege0;
                std::vector<bits_t> buffer;
                messege0.resize(2);

                bits_t marker=1;
                messege0[0]=marker;
                messege0[1]=a;


                MPI_Status stat;
                MPI_Request req;
                bits_t source;
                bits_t size_count=0;

                int rank, p;
                MPI_Comm_size(MPI_COMM_WORLD, &p);
                MPI_Comm_rank(MPI_COMM_WORLD, &rank);
                                
                
//========================   s  e  n   d  =============================================================
                
                        
                if(send_count <p){
                    
                    MPI_Isend(&messege0[0],2,MPI_UNSIGNED_LONG,send_count ,123,MPI_COMM_WORLD,&req);

                }
                
//================ > p send =====================                
                
                else{

                    rankcycle = (rankcycle+1)%(p-1);

                    MPI_Isend(&messege0[0],2,MPI_UNSIGNED_LONG,rankcycle+1,123,MPI_COMM_WORLD,&req);//use 123 for master to worker results transfer
                    
                }
                            
            }
                        
            struct TreeNode temp = ll.back();
            ll.pop_back();
            root = &temp;


            if (root->height < master_depth && root->sum < d)
            {
                struct TreeNode right;
                right.val = root->val;
                right.height = root->height +1;
                right.sum = root->sum;
                root = &right;

                ll.push_back(*root); 
            }          

}
    

    if (send_count < p - 1) {
        for (bits_t i = 0; i< p - 1 - send_count; i++) {

            std::vector<bits_t> messege0;
            messege0.resize(2);
            bits_t also_marker = 1;
            messege0[0]=also_marker;
            bits_t non_sense = 8888;
            messege0[1]=non_sense;
            MPI_Request req;
            MPI_Isend(&messege0[0],2,MPI_UNSIGNED_LONG,i + send_count + 1,123,MPI_COMM_WORLD,&req);
        }
        send_count = p - 1;
  
    }
    
    //======================== send p terminate message==========
  
    
    for (int i = 0; i < p-1; i++) {

        send_count++;
        std::vector<bits_t> messege0;
        std::vector<bits_t> buffer;
        messege0.resize(2);
        bits_t marker=0;
        messege0[0]=marker;
        messege0[1]=marker;
        MPI_Status stat;//used to check message source
        MPI_Request req;
        bits_t source;
        bits_t size_count=0;
    
        MPI_Isend(&messege0[0],2,MPI_UNSIGNED_LONG,i+1,123,MPI_COMM_WORLD,&req);//use 123 for master to worker

        buffer.resize(size_count);

    }
    
    //================receive all solutions=====================

    int flag = 2012;
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    std::vector<bits_t> messege_test;
    messege_test.resize( send_count + 1 );
    bits_t result_recv_test = 1999;
    
    bits_t receiveout = 0;
    MPI_Request req_4_recv;
    MPI_Status stat_4_recv;


    
    
   //========== master receive ================
    bits_t have;
    bits_t rev_count = 0;
    std::vector<bits_t> myresults;
    while(true){
        MPI_Recv(&have,1,MPI_UNSIGNED_LONG,MPI_ANY_SOURCE,123,MPI_COMM_WORLD,&stat);//r
        
        if (have == 0)
        {
            rev_count++;
        }
        if (rev_count == p-1)
        {
            break;
        }
        if(have == 0)continue;
        else{
        myresults.push_back(have);
         }
    }   


    
    
return myresults;
}
