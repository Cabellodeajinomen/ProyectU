    #ifndef HTML_PARSER_H
    #define HTML_PARSER_H

    #include <string>
    #include <vector>
    #include <memory>

    struct Node;
    struct Child {
        bool isText;               
        std::string text;          
        std::shared_ptr<Node> node;
        Child(const std::string &t) : isText(true), text(t) {}
        Child(std::shared_ptr<Node> n) : isText(false), node(n) {}
    };

    struct Node {
        std::string tag;           
        std::string tagLower;      
        std::string raw;           
        std::vector<Child> children; 
        bool selfClosing = false;  
        Node(const std::string &tg, const std::string &r, bool sc=false)
            : tag(tg), raw(r), selfClosing(sc) {
            tagLower = tg;
            for (auto &ch : tagLower) ch = (char)tolower((unsigned char)ch);
        }
    };

    class HtmlParser {
    private:
        HtmlParser();
        std::shared_ptr<Node> root;
        std::vector<std::shared_ptr<Node>> stack;
    long long textChars = 0;
    long long tagPairs = 0;
    long long lines = 1; 

    public:
        static HtmlParser &Instance();
        void OnOpenTag(const std::string &rawTag, bool selfClosing);
        void OnCloseTag(const std::string &rawTag);
        void OnText(const std::string &text);
        void AddLines(int n);
        void PrintResult();
        static std::string ExtractTagName(const std::string &rawTag);
        static std::string Trim(const std::string &s);
    };

    #endif
