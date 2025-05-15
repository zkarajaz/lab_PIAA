#include <bits/stdc++.h>
using namespace std;

#define DEBUG false

class Node {
public:
    char val;
    unordered_map<char, Node*> next;
    Node* parent;
    bool is_terminal;
    string word;
    Node* link;
    Node* word_link;
    unordered_map<char, Node*> moves;

    Node(char v, Node* p = nullptr, bool t = false) : val(v), parent(p), is_terminal(t), link(nullptr), word_link(nullptr) {}

    Node* get_link() {
        if (link) {
            if (DEBUG) cout << "Ссылка для узла '" << val << "' уже вычислена: " << link->val << endl;
            return link;
        }
        if (val == '\0') {
            link = this;
            if (DEBUG) cout << "Узел '" << val << "' является корневым, ссылка на себя" << endl;
        } else if (parent->val == '\0') {
            link = parent;
            if (DEBUG) cout << "Узел '" << val << "' является дочерним корня, ссылка на корень" << endl;
        } else {
            if (DEBUG) cout << "Вычисление ссылки для узла '" << val << "' через родителя '" << parent->val << "'" << endl;
            link = parent->get_link()->get_move(val);
            if (DEBUG) cout << "Ссылка для узла '" << val << "' установлена на '" << link->val << "'" << endl;
        }
        return link;
    }

    Node* get_move(char c) {
        if (moves.find(c) != moves.end()) {
            if (DEBUG) cout << "Переход по символу '" << c << "' из узла '" << val << "' уже вычислен: " << moves[c]->val << endl;
            return moves[c];
        }
        if (next.find(c) != next.end()) {
            moves[c] = next[c];
            if (DEBUG) cout << "Переход по символу '" << c << "' из узла '" << val << "' найден: " << moves[c]->val << endl;
        } else if (val != '\0') {
            if (DEBUG) cout << "Переход по символу '" << c << "' из узла '" << val << "' не найден, вычисление через ссылку" << endl;
            moves[c] = get_link()->get_move(c);
            if (DEBUG) cout << "Переход по символу '" << c << "' из узла '" << val << "' установлен на '" << moves[c]->val << "'" << endl;
        } else {
            moves[c] = this;
            if (DEBUG) cout << "Узел '" << val << "' является корневым, переход по символу '" << c << "' на себя" << endl;
        }
        return moves[c];
    }

    Node* get_word_link() {
        if (word_link) {
            if (DEBUG) cout << "Ссылка на слово для узла '" << val << "' уже вычислена: " << word_link->val << endl;
            return word_link;
        }
        Node* l = get_link();
        if (DEBUG) cout << "Вычисление ссылки на слово для узла '" << val << "' через ссылку '" << l->val << "'" << endl;
        if (l->val == '\0' || l->is_terminal) {
            word_link = l;
            if (DEBUG) cout << "Ссылка на слово для узла '" << val << "' установлена на '" << word_link->val << "'" << endl;
        } else {
            if (DEBUG) cout << "Рекурсивное вычисление ссылки на слово для узла '" << val << "'" << endl;
            word_link = l->get_word_link();
            if (DEBUG) cout << "Ссылка на слово для узла '" << val << "' установлена на '" << word_link->val << "'" << endl;
        }
        return word_link;
    }
};

class Vertex {
public:
    Node* _root;
    unordered_map<string, int> _words;
    int _len;

    Vertex() : _root(new Node('\0')), _len(0) {}

    int size() { return _len; }

    void append(const string& word) {
        if (_words.find(word) != _words.end()) {
            if (DEBUG) cout << "Слово '" << word << "' уже добавлено, пропуск" << endl;
            return;
        }
        Node* node = _root;
        if (DEBUG) cout << "Добавление слова '" << word << "'" << endl;
        for (char c : word) {
            if (node->next.find(c) != node->next.end()) {
                if (DEBUG) cout << "Переход по символу '" << c << "' из узла '" << node->val << "'" << endl;
                node = node->next[c];
            } else {
                if (DEBUG) cout << "Создание нового узла для символа '" << c << "' в узле '" << node->val << "'" << endl;
                node->next[c] = new Node(c, node);
                node = node->next[c];
            }
        }
        node->is_terminal = true;
        node->word = word;
        _len++;
        _words[word] = _len;
        if (DEBUG) cout << "Слово '" << word << "' добавлено, терминальный узел: '" << node->val << "'" << endl;
    }

    void extend(const vector<string>& words) {
        if (DEBUG) cout << "Добавление списка слов: ";
        if (DEBUG) {
            cout << "[";
            for (size_t i = 0; i < words.size(); ++i) {
                cout << "\"" << words[i] << "\"";
                if (i < words.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        for (const auto& w : words) append(w);
    }

    vector<pair<int, int>> find(const string& text) {
        if (DEBUG) cout << "Поиск в тексте: '" << text << "'" << endl;
        vector<pair<int, int>> answer;
        Node* node = _root;
        for (int i = 0; i < text.size(); i++) {
            if (DEBUG) cout << "Обработка символа '" << text[i] << "' на позиции " << i << endl;
            node = node->get_move(text[i]);
            if (node->val == '\0') {
                if (DEBUG) cout << "Узел '" << node->val << "' является корневым, пропуск" << endl;
                continue;
            }
            if (node->is_terminal) {
                if (DEBUG) cout << "Найден терминальный узел '" << node->val << "' для слова '" << node->word << "'" << endl;
                answer.emplace_back(i + 2 - node->word.size(), _words[node->word]);
            }
            Node* u = node->get_word_link();
            while (u->val != '\0') {
                if (DEBUG) cout << "Переход по ссылке на слово для узла '" << u->val << "'" << endl;
                answer.emplace_back(i + 2 - u->word.size(), _words[u->word]);
                u = u->get_word_link();
            }
        }
        if (DEBUG) {
            cout << "Результат поиска: [";
            for (size_t i = 0; i < answer.size(); ++i) {
                cout << "(" << answer[i].first << ", " << answer[i].second << ")";
                if (i < answer.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        sort(answer.begin(), answer.end());
        return answer;
    }

    int max_outgoing_edges() {
        int max_edges = 0;
        queue<Node*> q;
        set<Node*> visited;
        q.push(_root);
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            if (visited.find(node) != visited.end()) continue;
            visited.insert(node);
            int edges = node->next.size();
            max_edges = max(max_edges, edges);
            if (DEBUG) cout << "Узел '" << node->val << "' имеет " << edges << " исходящих дуг" << endl;
            for (auto& p : node->next) q.push(p.second);
        }
        if (DEBUG) cout << "Максимальное количество исходящих дуг: " << max_edges << endl;
        return max_edges;
    }

    string remove_patterns(const string& text) {
        auto matches = find(text);
        if (matches.empty()) {
            if (DEBUG) cout << "Образцы не найдены, возвращается исходная строка: '" << text << "'" << endl;
            return text;
        }
        set<int> ban;
        for (auto& m : matches) {
            string y;
            for (auto& p : _words) {
                if (m.second == p.second) {
                    y = p.first;
                    break;
                }
            }
            for (int i = m.first - 1; i < m.first + y.size() - 1; i++) {
                ban.insert(i);
            }
        }
        string result;
        for (int i = 0; i < text.size(); i++) {
            if (ban.find(i) == ban.end()) result += text[i];
        }
        return result;
    }
};

int main() {
    string T;
    cin >> T;
    int n;
    cin >> n;
    vector<string> P(n);
    for (int i = 0; i < n; i++) cin >> P[i];
    Vertex vertex;
    vertex.extend(P);
    for (auto& include : vertex.find(T)) {
        cout << include.first << " " << include.second << endl;
    }
    cout << vertex.max_outgoing_edges() << endl;
    cout << vertex.remove_patterns(T) << endl;
    return 0;
}