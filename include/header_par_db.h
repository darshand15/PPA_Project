#ifndef HEADER_PAR_DB_H
#define HEADER_PAR_DB_H

#include <boost/preprocessor.hpp>
#include <boost/pfr.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <mutex>
#include <functional>

#define EXPAND(x) x
#define TO_STRING(x) #x
#define TO_STRING_EXPAND(x) TO_STRING(x)


template <typename T>
class BTreeNode 
{
    T *keys;
    int t;
    BTreeNode<T> **C;
    int n;
    bool leaf;

public:
    BTreeNode(int _t, bool _leaf) : t(_t), leaf(_leaf), n(0) 
    {
        keys = new T[2 * t - 1];
        C = new BTreeNode<T>*[2 * t]();
    }

    ~BTreeNode() 
    {
        delete[] keys;
        for (int i = 0; i <= n; ++i) 
        {
            delete C[i];
        }
        delete[] C;
    }

    void traverse() 
    {
        for (int i = 0; i < n; ++i) 
        {
            if (!leaf) 
            {
                C[i]->traverse();
            }
            // std::cout << " " << keys[i];
            boost::pfr::for_each_field(*(keys[i]), [](const auto& field) {
                std::cout << field << "; ";
            });
            std::cout << "\n";
        }
        if (!leaf) C[n]->traverse();
    }

    std::pair<BTreeNode<T>*, T> search_eq(const T &k, std::function<bool(const T&, const T&)> comp);
    void insertNonFull(const T &k, std::function<bool(const T&, const T&)> comp);
    void splitChild(int i, BTreeNode<T> *y, std::function<bool(const T&, const T&)> comp);
    BTreeNode<T>* split_ge(const T &x, std::function<bool(const T&, const T&)> comp);
    BTreeNode<T>* split_le(const T &y, std::function<bool(const T&, const T&)> comp);
    void range_traverse(const T &ge_val, const T &le_val, std::function<bool(const T&, const T&)> comp);

    template <typename U>
    friend class BTree;
};

template <typename T>
class BTree 
{
    BTreeNode<T> *root;
    int t;

public:
    BTree(int _t) : root(nullptr), t(_t) {}
    ~BTree() { delete root; }

    void traverse() 
    {
        if (root) 
        {
            root->traverse();
        }
        std::cout << "\n";
    }

    void insert(const T &k, std::function<bool(const T&, const T&)> comp);

    std::pair<BTreeNode<T>*, T> search_eq(const T &k, std::function<bool(const T&, const T&)> comp) 
    {
        if (!root) 
        {
            return {nullptr, T()};
        }
        return root->search_eq(k, comp);
    }

    BTree<T> range_query(const T &x, const T &y, std::function<bool(const T&, const T&)> comp) 
    {
        BTree<T> ge_tree = split_ge(x, comp);
        return ge_tree.split_le(y, comp);
    }

    void range_traverse(const T &ge_val, const T &le_val, std::function<bool(const T&, const T&)> comp)
    {
        if(root)
        {
            root->range_traverse(ge_val, le_val, comp);
        }
    }


private:
    BTree<T> split_ge(const T &x, std::function<bool(const T&, const T&)> comp) 
    {
        BTree<T> new_tree(t);
        if (root) 
        {
            new_tree.root = root->split_ge(x, comp);
        }
        return new_tree;
    }

    BTree<T> split_le(const T &y, std::function<bool(const T&, const T&)> comp) 
    {
        BTree<T> new_tree(t);
        if (root) 
        {
            new_tree.root = root->split_le(y, comp);
        }
        return new_tree;
    }
};

template <typename T>
void BTree<T>::insert(const T &k, std::function<bool(const T&, const T&)> comp) 
{
    if (!root) 
    {
        root = new BTreeNode<T>(t, true);
        root->keys[0] = k;
        root->n = 1;
    } 
    else 
    {
        if (root->n == 2 * t - 1) 
        {
            BTreeNode<T> *s = new BTreeNode<T>(t, false);
            s->C[0] = root;
            s->splitChild(0, root, comp);
            int i = 0;
            if (comp(s->keys[0], k)) i++;
            s->C[i]->insertNonFull(k, comp);
            root = s;
        } 
        else 
        {
            root->insertNonFull(k, comp);
        }
    }
}

template <typename T>
void BTreeNode<T>::insertNonFull(const T &k, std::function<bool(const T&, const T&)> comp) 
{
    int i = n - 1;
    if (leaf) 
    {
        while (i >= 0 && comp(k, keys[i])) 
        {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    } 
    else 
    {
        while (i >= 0 && comp(k, keys[i])) i--;

        if (C[i + 1]->n == 2 * t - 1) 
        {
            splitChild(i + 1, C[i + 1], comp);
            if (comp(keys[i + 1], k)) i++;
        }
        C[i + 1]->insertNonFull(k, comp);
    }
}

template <typename T>
void BTreeNode<T>::splitChild(int i, BTreeNode<T> *y, std::function<bool(const T&, const T&)> comp) 
{
    BTreeNode<T> *z = new BTreeNode<T>(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) 
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n++;
}

template <typename T>
BTreeNode<T>* BTreeNode<T>::split_ge(const T &x, std::function<bool(const T&, const T&)> comp) 
{
    BTreeNode<T>* new_node = new BTreeNode<T>(t, leaf);
    int i = 0;
    while (i < n && comp(keys[i], x)) i++;

    if (leaf) 
    {
        new_node->n = n - i;
        for (int j = 0; j < new_node->n; j++)
        {
            new_node->keys[j] = keys[i + j];
        }

        return new_node->n > 0 ? new_node : nullptr;
    } 
    else 
    {
        int new_child_idx = 0;
        BTreeNode<T>* first_child = C[i] ? C[i]->split_ge(x, comp) : nullptr;
        
        if (first_child) 
        {
            new_node->C[new_child_idx++] = first_child;
            for (int j = i; j < n; j++) 
            {
                new_node->keys[new_node->n++] = keys[j];
                if (C[j + 1]) 
                {
                    BTreeNode<T>* child_part = C[j + 1]->split_ge(x, comp);
                    if (child_part) 
                    {
                        new_node->C[new_child_idx++] = child_part;
                    }
                }
            }
        }

        // Only one child
        if (new_node->n == 0 && new_child_idx == 1) 
        {
            BTreeNode<T>* only_child = new_node->C[0];
            delete new_node;
            return only_child;
        }

        return (new_node->n > 0 || new_child_idx > 0) ? new_node : nullptr;
    }
}

template <typename T>
BTreeNode<T>* BTreeNode<T>::split_le(const T &y, std::function<bool(const T&, const T&)> comp) 
{
    BTreeNode<T>* new_node = new BTreeNode<T>(t, leaf);
    int i = 0;
    while (i < n && !comp(y, keys[i])) i++;

    if (leaf) 
    {
        new_node->n = i;
        for (int j = 0; j < i; j++)
        {
            new_node->keys[j] = keys[j];
        }

        return new_node->n > 0 ? new_node : nullptr;
    } 
    else 
    {
        int new_child_idx = 0;
        for (int j = 0; j < i; j++) 
        {
            if (C[j]) 
            {
                BTreeNode<T>* child_part = C[j]->split_le(y, comp);
                if (child_part) 
                {
                    new_node->C[new_child_idx++] = child_part;
                    new_node->keys[new_node->n++] = keys[j];
                }
            }
        }

        // Process last child
        if (C[i]) 
        {
            BTreeNode<T>* last_child = C[i]->split_le(y, comp);
            if (last_child) 
            {
                new_node->C[new_child_idx++] = last_child;
            }
        }

        // Nodes with children but no keys
        if (new_node->n == 0 && new_child_idx > 0) 
        {
            BTreeNode<T>* only_child = new_node->C[0];
            delete new_node;
            return only_child;
        }

        return (new_node->n > 0 || new_child_idx > 0) ? new_node : nullptr;
    }
}

template <typename T>
std::pair<BTreeNode<T>*, T> BTreeNode<T>::search_eq(const T &k, std::function<bool(const T&, const T&)> comp) 
{
    int i = 0;
    while (i < n && comp(keys[i], k)) i++;
    
    if (i < n && !comp(k, keys[i]) && !comp(keys[i], k))
        return {this, keys[i]};
    
    if (leaf)
        return {nullptr, T()};
    
    return C[i]->search_eq(k, comp);
}

template <typename T>
void BTreeNode<T>::range_traverse(const T &ge_val, const T &le_val, std::function<bool(const T&, const T&)> comp) 
{
    int i = 0;

    while (i < n && comp(keys[i], ge_val)) 
    {
        i++;
    }

    while (i < n && !comp(le_val, keys[i]))
    {
        if (!leaf) 
        {
            C[i]->range_traverse(ge_val, le_val, comp);
        }
        // std::cout << " " << keys[i];
        boost::pfr::for_each_field(*(keys[i]), [](const auto& field) {
            std::cout << field << "; ";
        });
        std::cout << "\n";
        ++i;
    }

    if (!leaf) 
    {
        C[i]->range_traverse(ge_val, le_val, comp);
    }
}




#define FIELD(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem);

#define CREATE_TABLE_STRUCT(name, fields) \
    struct row_##name \
    { \
        BOOST_PP_SEQ_FOR_EACH(FIELD, _, BOOST_PP_VARIADIC_TO_SEQ fields) \
    };\
    typedef struct row_##name row_##name;

#define PUSH_ATTR_NAME(r, data, elem) \
    data.push_back(TO_STRING_EXPAND(BOOST_PP_TUPLE_ELEM(2, 1, elem)));


#define KEY_BOOL_STRUCT(r, data, elem) \
    bool BOOST_PP_TUPLE_ELEM(2, 1, elem);

#define KEY_BOOL_STRUCT_INIT_ALL_FALSE(r, data, elem) \
    data.BOOST_PP_TUPLE_ELEM(2, 1, elem) = false;

#define CREATE_KEY_STRUCT(name, fields) \
    struct key_bool_type_##name \
    { \
        BOOST_PP_SEQ_FOR_EACH(KEY_BOOL_STRUCT, _, BOOST_PP_VARIADIC_TO_SEQ fields) \
    };\
    typedef struct key_bool_type_##name key_bool_type_##name; \
    key_bool_type_##name key_bool_var_##name[2]; \
    BOOST_PP_SEQ_FOR_EACH(KEY_BOOL_STRUCT_INIT_ALL_FALSE, key_bool_var_##name[0], BOOST_PP_VARIADIC_TO_SEQ fields) \
    BOOST_PP_SEQ_FOR_EACH(KEY_BOOL_STRUCT_INIT_ALL_FALSE, key_bool_var_##name[1], BOOST_PP_VARIADIC_TO_SEQ fields)


#define CREATE_TABLE(name, fields) \
    CREATE_TABLE_STRUCT(name, fields) \
    std::vector<row_##name*> name; \
    std::vector<std::string> attr_##name; \
    BOOST_PP_SEQ_FOR_EACH(PUSH_ATTR_NAME, attr_##name, BOOST_PP_VARIADIC_TO_SEQ fields) \
    CREATE_KEY_STRUCT(name, fields) \
    BTree<row_##name*> pk_btree_##name(3);


#define PK_CMP_ELSE_IF(r, data, elem) \
    else if(left->BOOST_PP_TUPLE_ELEM(1, 0, elem) != right->BOOST_PP_TUPLE_ELEM(1, 0, elem)) \
    { \
        return left->BOOST_PP_TUPLE_ELEM(1, 0, elem) < right->BOOST_PP_TUPLE_ELEM(1, 0, elem); \
    }

#define CREATE_PK_CMP(name, cols) \
    auto cmp_##name = [](row_##name* left, row_##name* right) \
    { \
        if(false){}\
        BOOST_PP_SEQ_FOR_EACH(PK_CMP_ELSE_IF, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    };


#define KEY_BOOL_STRUCT_INIT_TRUE(r, data, elem) \
    data.BOOST_PP_TUPLE_ELEM(1, 0, elem) = true;

#define SET_PRIMARY_KEY(name, cols) \
    BOOST_PP_SEQ_FOR_EACH(KEY_BOOL_STRUCT_INIT_TRUE, key_bool_var_##name[0], BOOST_PP_VARIADIC_TO_SEQ cols) \
    CREATE_PK_CMP(name, cols)


#define TEST_BOOL_PRIMARY_KEY(r, data, elem) \
    if(temp && BOOST_PP_TUPLE_ELEM(3, 1, data).BOOST_PP_TUPLE_ELEM(2, 0, elem)) \
    { \
        if(BOOST_PP_TUPLE_ELEM(3, 0, data)->BOOST_PP_TUPLE_ELEM(2, 0, elem) == BOOST_PP_TUPLE_ELEM(2, 1, elem)) \
        { \
            BOOST_PP_TUPLE_ELEM(3, 2, data) = true; \
        } \
        else \
        { \
            BOOST_PP_TUPLE_ELEM(3, 2, data) = false; \
            temp = false; \
        } \
    }
     


#define ENFORCE_PRIMARY_KEY_CONSTRAINT(name, values, primary_key_constraint_viol) \
    for(auto row_trav : name) \
    { \
        bool temp = true; \
        BOOST_PP_SEQ_FOR_EACH(TEST_BOOL_PRIMARY_KEY, (row_trav, key_bool_var_##name[0], primary_key_constraint_viol), BOOST_PP_VARIADIC_TO_SEQ values) \
        if(primary_key_constraint_viol) \
        { \
            break; \
        } \
    }


#define INSERT_FIELDS_INTO_ROW(r, data, elem) \
    data->BOOST_PP_TUPLE_ELEM(2, 0, elem) = BOOST_PP_TUPLE_ELEM(2, 1, elem);

#define INSERT_INTO(name, values) \
    { \
        row_##name *tab_row = new row_##name; \
        BOOST_PP_SEQ_FOR_EACH(INSERT_FIELDS_INTO_ROW, tab_row, BOOST_PP_VARIADIC_TO_SEQ values) \
        bool primary_key_constraint_viol = false; \
        ENFORCE_PRIMARY_KEY_CONSTRAINT(name, values, primary_key_constraint_viol) \
        if(!primary_key_constraint_viol) \
        { \
            name.push_back(tab_row); \
            pk_btree_##name.insert(tab_row, cmp_##name); \
        } \
        else \
        { \
            std::cout << "Insertion of row skipped as Primary Key Constraint would be violated\n"; \
        } \
    }

#define SELECT_ALL(name) \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    }



#define SELECT_PK_EQ(name, cond_lhs, cond_cmp, cond_rhs) \
{ \
    row_##name *search_eq_row = new row_##name; \
    search_eq_row->cond_lhs = cond_rhs; \
    auto ret_search = pk_btree_##name.search_eq(search_eq_row, cmp_##name); \
    if(ret_search.first != nullptr) \
    { \
        for(auto ele_attr : attr_##name) \
        { \
            std::cout << ele_attr << "; "; \
        } \
        std::cout << "\n"; \
        boost::pfr::for_each_field(*(ret_search.second), [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    } \
    else \
    { \
        std::cout << "No matching Tuple found\n"; \
    } \
}

#define SELECT_PK_EQ_SEQ(name, field_pk, cond_cmp, eq_val) \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        if(row_trav->field_pk == eq_val) \
        { \
            boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
                std::cout << field << "; "; \
            }); \
            std::cout << "\n"; \
            break;\
        } \
    }

#define SELECT_ALL_RANGE(name, field_pk, ge_val, le_val) \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        if(row_trav->field_pk >= ge_val && row_trav->field_pk <= le_val) \
        { \
            boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
                std::cout << field << "; "; \
            }); \
            std::cout << "\n"; \
        } \
    }



#define SELECT_PK_RANGE(name, field_pk, ge_val, le_val) \
    row_##name *range_ge_row = new row_##name; \
    range_ge_row->field_pk = ge_val; \
    row_##name *range_le_row = new row_##name; \
    range_le_row->field_pk = le_val; \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    pk_btree_##name.range_traverse(range_ge_row, range_le_row, cmp_##name); \
    



#define SELECT_ALL_COND(name, cond_lhs, cond_cmp, cond_rhs) \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        if(row_trav->cond_lhs cond_cmp cond_rhs) \
        { \
            boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
                std::cout << field << "; "; \
            }); \
            std::cout << "\n"; \
        } \
    }


#define SELECT_ALL_COND_PAR(name, cond_lhs, cond_cmp, cond_rhs) \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    { \
        std::vector<int> print_ind; \
        print_ind.reserve(name.size()); \
        std::mutex print_ind_mutex; \
        parlay::parallel_for(0, name.size(), [&](size_t ind){ \
            if((name[ind])->cond_lhs cond_cmp cond_rhs) \
            { \
                print_ind_mutex.lock(); \
                print_ind.push_back(ind); \
                print_ind_mutex.unlock(); \
            } \
            return 0; \
        }); \
        \
        for(int i = 0; i<print_ind.size(); ++i) \
        { \
            boost::pfr::for_each_field(*(name[print_ind[i]]), [](const auto& field) { \
                std::cout << field << "; "; \
            }); \
            std::cout << "\n"; \
        } \
    }
    
    


#define DISPLAY_COL_NAMES(r, data, elem) \
    std::cout << TO_STRING_EXPAND(BOOST_PP_TUPLE_ELEM(1, 0, elem)) << "; ";

#define DISPLAY_VALS_COLS(r, data, elem) \
    std::cout << data->BOOST_PP_TUPLE_ELEM(1, 0, elem) << "; ";

#define SELECT_COLS(name, cols) \
    BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, row_trav, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
    }


#define SELECT_COLS_COND(name, cols, cond_lhs, cond_cmp, cond_rhs) \
    BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
    std::cout << "\n"; \
    for(auto row_trav : name) \
    { \
        if(row_trav->cond_lhs cond_cmp cond_rhs) \
        { \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, row_trav, BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
    }

#define SELECT_COLS_COND_PAR(name, cols, cond_lhs, cond_cmp, cond_rhs) \
    BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
    std::cout << "\n"; \
    { \
        std::vector<int> print_ind; \
        print_ind.reserve(name.size()); \
        std::mutex print_ind_mutex; \
        parlay::parallel_for(0, name.size(), [&](size_t ind){ \
            if((name[ind])->cond_lhs cond_cmp cond_rhs) \
            { \
                print_ind_mutex.lock(); \
                print_ind.push_back(ind); \
                print_ind_mutex.unlock(); \
            } \
            return 0; \
        }); \
        \
        for(int i = 0; i<print_ind.size(); ++i) \
        { \
            if((name[print_ind[i]])->cond_lhs cond_cmp cond_rhs) \
            { \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, (name[print_ind[i]]), BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
            } \
        } \
    }


#define DELETE_ALL_ROWS(name) \
    for(auto row_trav : name) \
    { \
        delete row_trav; \
    } \
    name.clear();



#define DELETE_COND(name, cond_lhs, cond_cmp, cond_rhs) \
    auto it_trav = name.begin(); \
    while(it_trav != name.end()) \
    { \
        if((*it_trav)->cond_lhs cond_cmp cond_rhs) \
        { \
            delete (*it_trav); \
            it_trav = name.erase(it_trav); \
        } \
        else \
        { \
            ++it_trav; \
        } \
    } \


#define UPDATE_ALL(name, upd_values) \
    for(auto row_trav : name) \
    { \
        BOOST_PP_SEQ_FOR_EACH(INSERT_FIELDS_INTO_ROW, row_trav, BOOST_PP_VARIADIC_TO_SEQ upd_values) \
    }


#define UPDATE_COND(name, upd_values, cond_lhs, cond_cmp, cond_rhs) \
    for(auto row_trav : name) \
    { \
        if(row_trav->cond_lhs cond_cmp cond_rhs) \
        { \
            BOOST_PP_SEQ_FOR_EACH(INSERT_FIELDS_INTO_ROW, row_trav, BOOST_PP_VARIADIC_TO_SEQ upd_values) \
        } \
    }


// #define GENERATE_SORT_CMP(name, field) \
//     [](row_##name* left, row_##name* right) { return left->field < right->field; }


#define GENERATE_SORT_CMP_ASC(field) \
    if(left->field != right->field) \
    { \
        return left->field < right->field; \
    } \

#define EXPAND_FIELD_COMPARISON_ASC(r, data, elem) GENERATE_SORT_CMP_ASC elem

#define GENERATE_SORT_CMP_DESC(field) \
    if(left->field != right->field) \
    { \
        return left->field > right->field; \
    } \

#define EXPAND_FIELD_COMPARISON_DESC(r, data, elem) GENERATE_SORT_CMP_DESC elem


#define ORDER_BY_ASC(name, cols) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : sort_vec) \
    { \
        boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    } \
    std::cout << "\n"; \
}

#define ORDER_BY_DESC(name, cols) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_DESC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : sort_vec) \
    { \
        boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    } \
    std::cout << "\n"; \
}

#define GENERATE_EQUALITY_CMP_GROUP_BY(r, data, elem) \
    && first_r->BOOST_PP_TUPLE_ELEM(1, 0, elem) == data->BOOST_PP_TUPLE_ELEM(1, 0, elem)

#define GROUP_BY_COUNT(name, cols) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_vec.size() != 0) \
    { \
        std::cout << "Count; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_vec[0]; \
        long long int curr_count = 1; \
        for(int i = 1; i<sort_vec.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_vec[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                ++curr_count; \
            } \
            else \
            { \
                std::cout << curr_count << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_count = 1; \
                first_r = sort_vec[i]; \
            } \
        } \
        std::cout << curr_count << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_MIN(name, cols, min_col) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_vec.size() != 0) \
    { \
        std::cout << "Min_" << TO_STRING_EXPAND(min_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_vec[0]; \
        auto min_val = sort_vec[0]->min_col; \
        for(int i = 1; i<sort_vec.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_vec[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                if(sort_vec[i]->min_col < min_val) \
                { \
                    min_val = sort_vec[i]->min_col; \
                } \
            } \
            else \
            { \
                std::cout << min_val << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                min_val = sort_vec[i]->min_col; \
                first_r = sort_vec[i]; \
            } \
        } \
        std::cout << min_val << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_MAX(name, cols, max_col) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_vec.size() != 0) \
    { \
        std::cout << "Max_" << TO_STRING_EXPAND(max_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_vec[0]; \
        auto max_val = sort_vec[0]->max_col; \
        for(int i = 1; i<sort_vec.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_vec[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                if(sort_vec[i]->max_col > max_val) \
                { \
                    max_val = sort_vec[i]->max_col; \
                } \
            } \
            else \
            { \
                std::cout << max_val << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                max_val = sort_vec[i]->max_col; \
                first_r = sort_vec[i]; \
            } \
        } \
        std::cout << max_val << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}

#define GROUP_BY_SUM(name, cols, sum_col) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_vec.size() != 0) \
    { \
        std::cout << "Sum_" << TO_STRING_EXPAND(sum_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_vec[0]; \
        long long int curr_sum = sort_vec[0]->sum_col; \
        for(int i = 1; i<sort_vec.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_vec[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                curr_sum += sort_vec[i]->sum_col; \
            } \
            else \
            { \
                std::cout << curr_sum << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_sum = sort_vec[i]->sum_col; \
                first_r = sort_vec[i]; \
            } \
        } \
        std::cout << curr_sum << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_AVG(name, cols, avg_col) \
{ \
    std::vector<row_##name*> sort_vec(name.begin(), name.end()); \
    std::sort(sort_vec.begin(), sort_vec.end(), [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_vec.size() != 0) \
    { \
        std::cout << "Avg_" << TO_STRING_EXPAND(avg_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_vec[0]; \
        double curr_sum = sort_vec[0]->avg_col; \
        long long int curr_count = 1; \
        for(int i = 1; i<sort_vec.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_vec[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                curr_sum += sort_vec[i]->avg_col; \
                ++curr_count; \
            } \
            else \
            { \
                std::cout << (curr_sum/curr_count) << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_sum = sort_vec[i]->avg_col; \
                curr_count = 1; \
                first_r = sort_vec[i]; \
            } \
        } \
        std::cout << (curr_sum/curr_count) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


template <typename Range, typename Comparator>
void merge(Range in1, Range in2, Range out, Comparator cmp) 
{
    long n1 = in1.size();
    long n2 = in2.size();
    if (n1 + n2 < 1000) std::merge(in1.begin(), in1.end(),
                                    in2.begin(), in2.end(),
                                    out.begin(), cmp);
    else if (n1 == 0) { parlay::copy(in2, out); }
    else if (n2 == 0) { parlay::copy(in1, out); }
    else if (n1 < n2) merge(in2, in1, out, cmp);
    else {
    long mid2 = std::lower_bound(in2.begin(),in2.end(),in1[n1/2], cmp)-in2.begin();
    parlay::par_do(
        [&]() { merge(in1.cut(0, n1/2), in2.cut(0, mid2),
                        out.cut(0, n1/2 + mid2), cmp); },
        [&]() { merge(in1.cut(n1/2, n1), in2.cut(mid2,n2),
                        out.cut(n1/2 + mid2, n1 + n2), cmp); });
    }
}


template <typename Range, typename Comparator>
void merge_sort_(Range in, Range out, bool inplace, Comparator cmp) 
{
    long n = in.size();
    if (n < 100) // adjust n to switch to sequential sort for smaller blocks
    {
        std::stable_sort(in.begin(), in.end(), cmp);
        if (!inplace) parlay::copy(in, out);
    } 
    else 
    {
        parlay::par_do(
            [&] () {merge_sort_(in.cut(0, n/2), out.cut(0, n/2), !inplace, cmp);},
            [&] () {merge_sort_(in.cut(n/2, n), out.cut(n/2, n), !inplace, cmp);});

        if (inplace)
        {
            merge(out.cut(0,n/2), out.cut(n/2, n), in.cut(0, n), cmp);
        }
        else 
        {
            merge(in.cut(0,n/2), in.cut(n/2, n), out.cut(0, n), cmp);
        }
    }
}


template <typename Range, typename Comparator>
void merge_sort(Range& in, Comparator cmp) 
{
    long n = in.size();
    using T = typename Range::value_type;
    parlay::sequence<T> tmp(n);
    merge_sort_(in.cut(0,n), tmp.cut(0,n), true, cmp);
}


#define ORDER_BY_ASC_PAR(name, cols) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : sort_seq) \
    { \
        boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    } \
    std::cout << "\n"; \
}


#define ORDER_BY_DESC_PAR(name, cols) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_DESC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    for(auto ele_attr : attr_##name) \
    { \
        std::cout << ele_attr << "; "; \
    } \
    std::cout << "\n"; \
    for(auto row_trav : sort_seq) \
    { \
        boost::pfr::for_each_field(*row_trav, [](const auto& field) { \
            std::cout << field << "; "; \
        }); \
        std::cout << "\n"; \
    } \
    std::cout << "\n"; \
}


#define GROUP_BY_COUNT_PAR(name, cols) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Count; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_seq[0]; \
        long long int curr_count = 1; \
        for(int i = 1; i<sort_seq.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_seq[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                ++curr_count; \
            } \
            else \
            { \
                std::cout << curr_count << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_count = 1; \
                first_r = sort_seq[i]; \
            } \
        } \
        std::cout << curr_count << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}

#define GENERATE_EQUALITY_CMP_GROUP_BY_PAR2(r, data, elem) \
    || BOOST_PP_TUPLE_ELEM(2, 0, data)->BOOST_PP_TUPLE_ELEM(1, 0, elem) != BOOST_PP_TUPLE_ELEM(2, 1, data)->BOOST_PP_TUPLE_ELEM(1, 0, elem)

#define GROUP_BY_COUNT_PAR2(name, cols) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    auto ind_seq = parlay::tabulate(sort_seq.size(), [](int i) -> int { return i; }); \
    auto boundaries_seq = parlay::filter(ind_seq, [&](auto ele) { return (ele == 0 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY_PAR2, (sort_seq[ele], sort_seq[ele-1]), BOOST_PP_VARIADIC_TO_SEQ cols)); }); \
    auto group_count_seq = parlay::tabulate(boundaries_seq.size(), [&](int i) -> int { \
        int st_ind = boundaries_seq[i]; \
        int en_ind = -1; \
        if(i + 1 == boundaries_seq.size()) \
        { \
            en_ind = sort_seq.size(); \
        } \
        else \
        { \
            en_ind = boundaries_seq[i+1]; \
        } \
        auto count_v = en_ind - st_ind; \
        return count_v; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Count; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        for(int i = 0; i<boundaries_seq.size(); ++i) \
        { \
            std::cout << group_count_seq[i] << "; "; \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, sort_seq[boundaries_seq[i]], BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_MIN_PAR(name, cols, min_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Min_" << TO_STRING_EXPAND(min_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_seq[0]; \
        auto min_val = sort_seq[0]->min_col; \
        for(int i = 1; i<sort_seq.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_seq[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                if(sort_seq[i]->min_col < min_val) \
                { \
                    min_val = sort_seq[i]->min_col; \
                } \
            } \
            else \
            { \
                std::cout << min_val << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                min_val = sort_seq[i]->min_col; \
                first_r = sort_seq[i]; \
            } \
        } \
        std::cout << min_val << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}

#define GROUP_BY_MIN_PAR2(name, cols, min_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    auto ind_seq = parlay::tabulate(sort_seq.size(), [](int i) -> int { return i; }); \
    auto min_col_seq = parlay::tabulate(sort_seq.size(), [&](int i) { return sort_seq[i]->min_col; }); \
    auto boundaries_seq = parlay::filter(ind_seq, [&](auto ele) { return (ele == 0 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY_PAR2, (sort_seq[ele], sort_seq[ele-1]), BOOST_PP_VARIADIC_TO_SEQ cols)); }); \
    auto group_min_seq = parlay::tabulate(boundaries_seq.size(), [&](int i) -> int { \
        int st_ind = boundaries_seq[i]; \
        int en_ind = -1; \
        if(i + 1 == boundaries_seq.size()) \
        { \
            en_ind = sort_seq.size(); \
        } \
        else \
        { \
            en_ind = boundaries_seq[i+1]; \
        } \
        auto m = parlay::make_monoid([](long long int left, long long int right) { \
            if(left <= right) \
            { \
                return left; \
            } \
            return right; \
        } ,LLONG_MAX); \
        auto min_v = parlay::reduce(min_col_seq.cut(st_ind, en_ind), m); \
        return min_v; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Min_" << TO_STRING_EXPAND(min_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        for(int i = 0; i<boundaries_seq.size(); ++i) \
        { \
            std::cout << group_min_seq[i] << "; "; \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, sort_seq[boundaries_seq[i]], BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
        std::cout << "\n\n"; \
    } \
}

#define GROUP_BY_MAX_PAR(name, cols, max_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Max_" << TO_STRING_EXPAND(max_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_seq[0]; \
        auto max_val = sort_seq[0]->max_col; \
        for(int i = 1; i<sort_seq.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_seq[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                if(sort_seq[i]->max_col > max_val) \
                { \
                    max_val = sort_seq[i]->max_col; \
                } \
            } \
            else \
            { \
                std::cout << max_val << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                max_val = sort_seq[i]->max_col; \
                first_r = sort_seq[i]; \
            } \
        } \
        std::cout << max_val << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}

#define GROUP_BY_MAX_PAR2(name, cols, max_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    auto ind_seq = parlay::tabulate(sort_seq.size(), [](int i) -> int { return i; }); \
    auto max_col_seq = parlay::tabulate(sort_seq.size(), [&](int i) { return sort_seq[i]->max_col; }); \
    auto boundaries_seq = parlay::filter(ind_seq, [&](auto ele) { return (ele == 0 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY_PAR2, (sort_seq[ele], sort_seq[ele-1]), BOOST_PP_VARIADIC_TO_SEQ cols)); }); \
    auto group_max_seq = parlay::tabulate(boundaries_seq.size(), [&](int i) -> int { \
        int st_ind = boundaries_seq[i]; \
        int en_ind = -1; \
        if(i + 1 == boundaries_seq.size()) \
        { \
            en_ind = sort_seq.size(); \
        } \
        else \
        { \
            en_ind = boundaries_seq[i+1]; \
        } \
        auto m = parlay::make_monoid([](long long int left, long long int right) { \
            if(left >= right) \
            { \
                return left; \
            } \
            return right; \
        } ,LLONG_MIN); \
        auto max_v = parlay::reduce(max_col_seq.cut(st_ind, en_ind), m); \
        return max_v; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Max_" << TO_STRING_EXPAND(max_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        for(int i = 0; i<boundaries_seq.size(); ++i) \
        { \
            std::cout << group_max_seq[i] << "; "; \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, sort_seq[boundaries_seq[i]], BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
        std::cout << "\n\n"; \
    } \
}

#define GROUP_BY_SUM_PAR(name, cols, sum_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Sum_" << TO_STRING_EXPAND(sum_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_seq[0]; \
        long long int curr_sum = sort_seq[0]->sum_col; \
        for(int i = 1; i<sort_seq.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_seq[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                curr_sum += sort_seq[i]->sum_col; \
            } \
            else \
            { \
                std::cout << curr_sum << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_sum = sort_seq[i]->sum_col; \
                first_r = sort_seq[i]; \
            } \
        } \
        std::cout << curr_sum << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_SUM_PAR2(name, cols, sum_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    auto ind_seq = parlay::tabulate(sort_seq.size(), [](int i) -> int { return i; }); \
    auto sum_col_seq = parlay::tabulate(sort_seq.size(), [&](int i) { return sort_seq[i]->sum_col; }); \
    auto boundaries_seq = parlay::filter(ind_seq, [&](auto ele) { return (ele == 0 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY_PAR2, (sort_seq[ele], sort_seq[ele-1]), BOOST_PP_VARIADIC_TO_SEQ cols)); }); \
    auto group_count_seq = parlay::tabulate(boundaries_seq.size(), [&](int i) -> int { \
        int st_ind = boundaries_seq[i]; \
        int en_ind = -1; \
        if(i + 1 == boundaries_seq.size()) \
        { \
            en_ind = sort_seq.size(); \
        } \
        else \
        { \
            en_ind = boundaries_seq[i+1]; \
        } \
        auto m = parlay::make_monoid([](long long int left, long long int right) {return (left + right);} ,0); \
        auto sum_v = parlay::reduce(sum_col_seq.cut(st_ind, en_ind), m); \
        return sum_v; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Sum_" << TO_STRING_EXPAND(sum_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        for(int i = 0; i<boundaries_seq.size(); ++i) \
        { \
            std::cout << group_count_seq[i] << "; "; \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, sort_seq[boundaries_seq[i]], BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
        std::cout << "\n\n"; \
    } \
}




#define GROUP_BY_AVG_PAR(name, cols, avg_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Avg_" << TO_STRING_EXPAND(avg_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        row_##name* first_r = sort_seq[0]; \
        double curr_sum = sort_seq[0]->avg_col; \
        long long int curr_count = 1; \
        for(int i = 1; i<sort_seq.size(); ++i) \
        { \
            if(1 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY, sort_seq[i], BOOST_PP_VARIADIC_TO_SEQ cols)) \
            { \
                curr_sum += sort_seq[i]->avg_col; \
                ++curr_count; \
            } \
            else \
            { \
                std::cout << (curr_sum/curr_count) << "; "; \
                BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
                std::cout << "\n"; \
                curr_sum = sort_seq[i]->avg_col; \
                curr_count = 1; \
                first_r = sort_seq[i]; \
            } \
        } \
        std::cout << (curr_sum/curr_count) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, first_r, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n\n"; \
    } \
}


#define GROUP_BY_AVG_PAR2(name, cols, avg_col) \
{ \
    parlay::sequence<row_##name*> sort_seq(name.begin(), name.end()); \
    merge_sort(sort_seq, [](row_##name* left, row_##name* right){ \
        BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_COMPARISON_ASC, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        return false; \
    }); \
    auto ind_seq = parlay::tabulate(sort_seq.size(), [](int i) -> int { return i; }); \
    auto avg_col_seq = parlay::tabulate(sort_seq.size(), [&](int i) { return sort_seq[i]->avg_col; }); \
    auto boundaries_seq = parlay::filter(ind_seq, [&](auto ele) { return (ele == 0 BOOST_PP_SEQ_FOR_EACH(GENERATE_EQUALITY_CMP_GROUP_BY_PAR2, (sort_seq[ele], sort_seq[ele-1]), BOOST_PP_VARIADIC_TO_SEQ cols)); }); \
    auto group_avg_seq = parlay::tabulate(boundaries_seq.size(), [&](int i) -> int { \
        int st_ind = boundaries_seq[i]; \
        int en_ind = -1; \
        if(i + 1 == boundaries_seq.size()) \
        { \
            en_ind = sort_seq.size(); \
        } \
        else \
        { \
            en_ind = boundaries_seq[i+1]; \
        } \
        auto m = parlay::make_monoid([](long long int left, long long int right) {return (left + right);} ,0); \
        double sum_v = parlay::reduce(avg_col_seq.cut(st_ind, en_ind), m); \
        auto count_v = en_ind - st_ind; \
        double avg_v = sum_v/count_v; \
        return avg_v; \
    }); \
    if(sort_seq.size() != 0) \
    { \
        std::cout << "Avg_" << TO_STRING_EXPAND(avg_col) << "; "; \
        BOOST_PP_SEQ_FOR_EACH(DISPLAY_COL_NAMES, _, BOOST_PP_VARIADIC_TO_SEQ cols) \
        std::cout << "\n"; \
        for(int i = 0; i<boundaries_seq.size(); ++i) \
        { \
            std::cout << group_avg_seq[i] << "; "; \
            BOOST_PP_SEQ_FOR_EACH(DISPLAY_VALS_COLS, sort_seq[boundaries_seq[i]], BOOST_PP_VARIADIC_TO_SEQ cols) \
            std::cout << "\n"; \
        } \
        std::cout << "\n\n"; \
    } \
}

#endif // HEADER_PAR_DB_H