#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

class HtmlBuilder;
/*
Keep the HtmlElement constructor in private, so user can't create obj of it.
Force the user to use the builder.
*/
class HtmlElement {
    //For builder to call construct of the html element. making it friend.
    friend class HtmlBuilder;

    string name, text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2;

    HtmlElement() {}

    HtmlElement(const string& name, const string& text) : name(name), text(text) {}
public:
    string str(int indent = 0) const {
        ostringstream oss;
        string i(indent_size*indent, ' ');
        oss << i << "<" << name << ">" << endl;

        if(text.size() > 0)
            oss << string(indent_size*(indent+1), ' ') << text << endl;

        for(const auto& e : elements)
            oss << e.str(indent + 1);

        oss << i << "</" << name << ">" << endl;
        return oss.str();
    }

    static unique_ptr<HtmlBuilder> build(string root_name)
    {
        return make_unique<HtmlBuilder>(root_name);
    }
};

class HtmlBuilder {
public:
    HtmlElement root;

    HtmlBuilder(string root_name) {
        root.name = root_name;
    };

    HtmlBuilder& add_child(string child_name, string child_text) {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }

    HtmlElement build() { return root; }
    string str() const { return root.str(); }
    operator HtmlElement() const { return root; }
};

int main() {
    cout << "in Main" << endl;

    HtmlBuilder builder{"ul"};
    builder.add_child("li", "hello").add_child("li", "world");
    //builder.add_child("li", "hello");
    //builder.add_child("li", "world");
    cout << builder.str() << endl;

    auto builder2 = HtmlElement::build("ul")->add_child("li", "hello").add_child("li", "alag");
    cout << builder2.str() << endl;
}
