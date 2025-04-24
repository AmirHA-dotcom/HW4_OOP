#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

// Model & Data Base ===================================================================================================

enum class Node_Type { Drawing, Peg, Composite };
enum class Port_Type { Input, Output };
class Node;
class Wire;

struct way_point
{
    int x, y;
};

struct Port
{
    Node* node;
    Port_Type type;
    int index;
};

class Node
{
public:
    Node_Type type;
    int x, y;
    vector<Port> input_ports;
    vector<Port> output_ports;

    Node(Node_Type t, int posX, int posY) : type(t), x(posX), y(posY)
    {
        if (type == Node_Type::Drawing)
        {
            input_ports.push_back({this, Port_Type::Input, 0});
        }
        else if (type == Node_Type::Peg)
        {
            input_ports.push_back({this, Port_Type::Input, 0});
            output_ports.push_back({this, Port_Type::Output, 0});
        }
        else if (type == Node_Type::Composite)
        {
            // No ports by default, user should add as needed!!!!!!
        }
    }

    void add_output_port()
    {
        output_ports.push_back({this, Port_Type::Output, (int)output_ports.size()});
    }

    void add_input_port()
    {
        input_ports.push_back({this, Port_Type::Input, (int)input_ports.size()});
    }
};

struct Wire
{
    Port* from;
    Port* to;
    vector<way_point> way_points;

    Wire(Port* f, Port* t) : from(f), to(t) {}
};


class Model
{
private:
    vector<Node*> nodes;
    vector<Wire*> wires;

public:
    ~Model()
    {
        for (auto n : nodes) delete n;
        for (auto w : wires) delete w;
    }

    Node* create_node(Node_Type type, int x, int y)
    {
        Node* node = new Node(type, x, y);
        nodes.push_back(node);
        return node;
    }

    Wire* create_wire(Port* from, Port* to)
    {
        if (!is_valid_connection(from, to)) return nullptr;

        Wire* wire = new Wire(from, to);
        wires.push_back(wire);
        return wire;
    }

    bool is_valid_connection(Port* from, Port* to)
    {
        if (from->type != Port_Type::Output || to->type != Port_Type::Input) return false;

        Node_Type fromType = from->node->type;
        Node_Type toType = to->node->type;

        if (toType == Node_Type::Drawing) return true;
        if (toType == Node_Type::Peg && fromType == Node_Type::Peg) return true;
        if (toType == Node_Type::Composite && fromType != Node_Type::Peg) return true;

        return false;
    }

    void delete_node(Node* node)
    {
        wires.erase(remove_if(wires.begin(), wires.end(), [node](Wire* w) {
            bool to_delete = (w->from->node == node || w->to->node == node);
            if (to_delete) delete w;
            return to_delete;
        }), wires.end());

        nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
        delete node;
    }

    const std::vector<Node*>& get_nodes() const { return nodes; }
    const std::vector<Wire*>& get_wires() const { return wires; }
};



// View ================================================================================================================

class View
{
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
public:
    View(SDL_Renderer* r, TTF_Font* f) : renderer(r), font(f){}

    void render(const Model& model)
    {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderClear(renderer);

// Work Place
        SDL_SetRenderDrawColor(renderer, 45, 45, 45,255);
        SDL_Rect Border{0, 700, 1800, 250};
        SDL_RenderFillRect(renderer, &Border);
        SDL_SetRenderDrawColor(renderer, 15, 15 ,15 ,255);
        SDL_Rect Node_Library{15, 715, 1770, 220};
        SDL_RenderFillRect(renderer, &Node_Library);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, 15, 715, 1785, 715);
        SDL_RenderDrawLine(renderer, 15, 935, 1785, 935);
        SDL_RenderDrawLine(renderer, 15, 715, 15, 935);
        SDL_RenderDrawLine(renderer, 1785, 715, 1785, 935);


// Node Library Text
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect Text1{20, 720, 200, 50};
        SDL_RenderFillRect(renderer, &Text1);
        SDL_Color textColor = {255, 255, 255};
        const char* text1 = "Node Library";
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text1, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int textW, textH;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect{Text1.x + (Text1.w - textW) / 2,Text1.y + (Text1.h - textH) / 2,textW,textH};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, 20, 720, 220, 720);
        SDL_RenderDrawLine(renderer, 20, 770, 220, 770);
        SDL_RenderDrawLine(renderer, 20, 720, 20, 770);
        SDL_RenderDrawLine(renderer, 20, 720, 20, 770);


        SDL_RenderPresent(renderer);
    }
};

// Controller ==========================================================================================================

class Controller
{
private:
    View& view;
    Model& model;

public:
    Controller(View& v, Model& m) : view(v), model(m) {}

    void handle_event(SDL_Event& event)
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w)
        {
            std::cout << "Add waypoint mode!" << std::endl;
            // model.add_waypoint(...);
        }
    }
};


// int main ============================================================================================================

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("AHA's Node Viewer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1800, 950,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("D:/Fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf", 24);


    // main loop variables
    Model model;
    View view(renderer, font);
    Controller controller(view, model);

    bool running = true;
    SDL_Event event;
// main loop ===========================================================================================================
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            controller.handle_event(event);

        }// poll event
        view.render(model);
        SDL_Delay(16);
    }// main loop

// End Program =========================================================================================================
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}