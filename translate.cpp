#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>

using namespace std;
#define CHAR_MAP_SIZE 27

struct trie_node 
{
	int count;
	string data = "";
	struct trie_node* kinder[CHAR_MAP_SIZE];
};

typedef struct trie_node* trie_t;

trie_t trie_create(void) 
{
	trie_t node = new trie_node;

	if (node == NULL) 
	{
		return NULL;
	}

	node->count = 0;

	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		node->kinder[i] = NULL;
	}

	return node;
}
void trie_free(trie_t tree) // FreeTree ***********
{ 
	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		if (tree->kinder[i] != NULL) 
		{
			trie_free(tree->kinder[i]);
		}
	}
	delete tree;
}

int char_to_map_index(char source)
{
	char tmp = tolower(source);
	if (tmp >= 'a' && tmp <= 'z') 
	{
		return tmp - 'a';
	}
	else if (tmp == char(39)) 
	{
		return 26;
	}
	else 
	{
		exit(2);
	}

}

char char_map_index_to_char(int char_map_index)
{
	if (char_map_index != 26) 
	{
		return 'a' + char_map_index;
	}
	else if (char_map_index == 26) 
	{
		return char(39);// '
	}
	return ' ';
}


string trie_get_first_word(trie_t root, string h = "")
{
	trie_t tmp = root;
	if (tmp == NULL) 
	{
		return " ";
	}
	for (int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		if (tmp->kinder[i] != NULL) 
		{
			h += char_map_index_to_char(i);
			if (tmp->kinder[i]->count > 0) 
			{
				return h;
			}
			return trie_get_first_word(tmp->kinder[i], h);
		}
	}
	return h;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string trie_get_next_word(int index_er, trie_t root, trie_t tmp, string current, string out = "", int count = 0)
{
	int g;
	if (count < current.length()) 
	{
		g = char_to_map_index(current[count]);
	}
	else 
	{
		g = 0;
	}
	count++;
	while (g < CHAR_MAP_SIZE) 
	{
		if (tmp->kinder[g] != NULL && (count == current.length() + 1 ? g > index_er:true)) 
		{
			out += char_map_index_to_char(g);
			if (tmp->kinder[g]->count > 0 && count > current.length()) 
			{
				return out;
			}
			return trie_get_next_word(index_er, root, tmp->kinder[g], current, out, count);
		}
		g++;
	}
	if (current == "") 
	{
		return "";
	}
	char m = current[current.length() - 1];
	current.pop_back();
	return trie_get_next_word(char_to_map_index(m), root, root, current);

}

void add(trie_t root, string n, string data0) 
{
	int l = n.length();
	trie_t tmp = root;
	for (int i = 0; i < l; i++) 
	{
		int g = char_to_map_index(n[i]);
		if (tmp->kinder[g] == NULL) 
		{
			tmp->kinder[g] = trie_create();
			tmp = tmp->kinder[g];
		}
		else 
		{
			tmp = tmp->kinder[g];
		}
	}
	tmp->count++;
	tmp->data = data0;

}

string find_words(trie_t root, string n) 
{
	trie_t tmp = root;
	if (tmp == NULL) 
	{
		return " ";
	}
	unsigned short int len = n.length();
	for (unsigned short int i = 0; i < len; i++) 
	{
		if (tmp == NULL) 
		{
			return " ";
		}
		unsigned short int g = char_to_map_index(n[i]);
		tmp = tmp->kinder[g];
	}
	if (tmp == NULL) 
	{
		return " ";
	}
	if (tmp->count > 0) 
	{
		return tmp->data;
	}
	else 
	{
		return " ";
	}

}

void draw(string* inp, trie_t root) 
{
	
	system("cls");
	
	cout << "INPUT" << "\t\t\t\t\t" << "WORDS" << endl;
	cout << *inp << endl;
	cout << "----------------------------------------------------------------------------------------------------------------" << endl;
	int i = 20;
	string word = (*inp == "") ? "a" : *inp;
	for (word = trie_get_next_word(-1, root, root, word); word != ""; word = trie_get_next_word(-1, root, root, word)) 
	{
		if (i == 0) { break; }
		i--;
		cout << "-\t\t\t\t\t" << word << endl;
	}
	cout << "TRANSLATE" << endl << find_words(root, *inp) << endl;
		
}

void input(string* f, trie_t root) 
{
	char c;
	string h = "";
	while (1) 
	{
		c = _getch();
		if (c == 8 && h.length() >= 1) 
		{
			h.erase(h.end() - 1);
		}
		else if (c != 8) 
		{
			h += c;
		}
		draw(&h, root);
	}
}




int main() 
{
	string inp = "";
	system("chcp 1251");
	
	ifstream  file;
	file.open("input.txt");
	if (!file.is_open()) 
	{
		cout << "eror" << endl;
		exit(1);
	}
	trie_t root = NULL;
	root = trie_create();

	cout << "LOAD...." << endl;
	string x, y;
	while (!file.eof()) 
	{
		file >> x;
		if (!file.eof()) 
		{
			file >> y;
			add(root, x, y);
		}
	}
	draw(&inp, root);
	input(&inp, root);
	
	file.close();
	system("pause");
	return 0;
}

/*13 Enter
27 escape
8 backspace
32 space

72 Стрелка вверх
80 Стрелка вниз
75 Стрелка влево
77 Стрелка вправо

115 ctrl-стрелка влево
116 ctrl-стрелка вправо

82 ins (вставка)
83 del (удалить)

73 pgup (страница вверх)
81 pgdn (страница вниз)
118 ctrl-pgdn (страница вниз)
132 ctrl - pgup (страница вверх)

71 home (начало)
79 end (конец)
119 ctrl-home (начало)
117 ctrl-end (конец)

15 shift tab
16-25 alt - q/w/e/r/t/y/u/i/o/p
30-38 alt - a/s/d/f/g/h/j/k/l
44-50 alt - z/x/c/v/b/n/m

59-68 Ключи f1-f10 (нельзя использовать клавиши с изменяемой функцией)
84-93 f11-f20 (shift-f1 - shift-f10)
94-103 f21-f30 (ctrl-f1 - ctrl-f10)

104-113 f31-f40 (alt-f1 - alt-f10)

133 f11
134 f12
135 shift-f11
136 shift-f13
137 ctrl-f11
138 ctrl-f12
139 alt-f11
140 alt-f12

114 ctrl-prtsc (копия с экрана)*/