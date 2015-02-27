#include "findmotifs.h"
#include "hamming.h"

struct TreeNode
{
    bits_t val;
    unsigned int sum;
    unsigned int height;

};

std::vector<bits_t> findmotifs(unsigned int n, unsigned int l, unsigned int d, const bits_t* input)
{
        std::vector<bits_t> result;

        std::vector<struct TreeNode> ll;
        //create start node
        struct TreeNode *root;
        struct TreeNode cur;
        cur.val = input[0];
        unsigned sum0 = 0;
        cur.sum = sum0;
        unsigned height0 = 0;
        cur.height = height0;
        root = &cur;
        //start node into stack
        ll.push_back(*root); 
        //add to result
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

   
        while(!ll.empty()){

            while(root->height < l && root->sum < d){  

                struct TreeNode left;

                left.height = (root->height) + 1;
                left.sum = (root->sum) + 1;
                left.val = root->val ^ (1<<(left.height)); 


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


            if (root->height < l && root->sum < d)
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