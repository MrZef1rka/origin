#include <iostream>
#include <string>
using namespace std;

class SimpleText
{
public:
    virtual void render(const string& text)
    {
        cout << text;
    }
};

class DecoratedText : public SimpleText
{
public:
    SimpleText* _text;
    DecoratedText(SimpleText* s_text) : _text(s_text) {}
};

class ItalicText : public DecoratedText
{
public:
    ItalicText(SimpleText* s_text) : DecoratedText(s_text) {}
    void render(const string& text) override
    {
        cout << "<i>";
        _text->render(text);
        cout << "</i>";
    }
};

class BoldText : public DecoratedText
{
public:
    BoldText(SimpleText* s_text) : DecoratedText(s_text) {}
    void render(const string& text) override
    {
        cout << "<b>";
        _text->render(text);
        cout << "</b>";
    }
};

class Paragraph : public DecoratedText
{
public:
    Paragraph(SimpleText* s_text) : DecoratedText(s_text) {}
    void render(const string& text) override
    {
        cout << "<p>";
        _text->render(text);
        cout << "</p>";
    }
    
};

class Reversed : public DecoratedText
{
public:
    Reversed(SimpleText* s_text) : DecoratedText(s_text) {}
    void render(const string& text) override
    {
        string reversed_text = text;
        std::reverse(reversed_text.begin(), reversed_text.end());
        _text->render(reversed_text);
    }
};

class Link : public DecoratedText
{
public:
    Link(SimpleText* s_text) : DecoratedText(s_text) {}
    
    void render(const string& url, const string& text)
    {
        cout << "<a href=\"" << url << "\">";
        _text->render(text);
        cout << "</a>";
    }
};


int main()
{

    SimpleText just_text;
    just_text.render("Hello World!");
    cout << endl;
    
    auto It_text = new ItalicText(&just_text);
    It_text->render("Hello World!");
    cout << endl;
    
    auto Bold_text = new BoldText(&just_text);
    Bold_text->render("Hello World!");
    cout << endl;
    
    auto Paragraph_text = new Paragraph(&just_text);
    Paragraph_text->render("Hello World!");
    cout << endl;

    auto Reverse_text = new Reversed(&just_text);
    Reverse_text->render("Hello World!");
    cout << endl;

    auto Link_text = new Link(&just_text);
    Link_text->render("netology.ru", "Hello world!");
    
    return 0;
}
