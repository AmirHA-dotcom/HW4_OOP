#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

// Functions ===========================================================================================================
bool click_in_rect(int x, int y, const SDL_Rect& rect)
{
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}
bool rects_overlap(const SDL_Rect& a, const SDL_Rect& b)
{
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}



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

// Drawing Node Icon
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect Drawing_Node{400, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Drawing_Node);
// Peg Node Icon
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect Peg_Node{700, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Peg_Node);
// Composite Node Icon
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect Composite_Node{1000, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Composite_Node);



        render_nodes(model);

        SDL_RenderPresent(renderer);
    }
    void render_nodes(const Model& model)
    {
        for (auto node : model.get_nodes())
        {
            SDL_Rect rect = {node->x, node->y, 100, 50};
            if (node->type == Node_Type::Drawing)
                SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
            else if (node->type == Node_Type::Peg)
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            else if (node->type == Node_Type::Composite)
                SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255);

            SDL_RenderFillRect(renderer, &rect);
        }
    }

};

// Controller ==========================================================================================================

class Controller
{
private:
    View& view;
    Model& model;
    bool dragging;
    Node* dragged_node;
    int offset_x, offset_y;
    int original_x;
    int original_y;
    bool new_node_dragged = false;

public:
    Controller(View& v, Model& m) : view(v), model(m) {dragging = false; dragged_node = nullptr; offset_x = 0; offset_y = 0; original_x = 0; original_y = 0;}

    void handle_event(SDL_Event& event)
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;
            SDL_Rect Node_Library_Button{20, 720, 200, 50};
            SDL_Rect drawing_node_button{400, 750, 150, 150};
            SDL_Rect peg_node_button{700, 750, 150, 150};
            SDL_Rect composite_node_button{1000, 750, 150, 150};
            for (auto node : model.get_nodes())
            {
                SDL_Rect node_rect{node->x, node->y, 100, 50};
                if (click_in_rect(mouse_x, mouse_y, node_rect))
                {
                    dragged_node = node;
                    dragging = true;
                    offset_x = mouse_x - node->x;
                    offset_y = mouse_y - node->y;
                    original_x = node->x;
                    original_y = node->y;
                    break;
                }
            }
            if (click_in_rect(mouse_x, mouse_y, Node_Library_Button))
            {
                cout << "Node Library Detected!!!" << endl;
            }
            else if (click_in_rect(mouse_x, mouse_y, drawing_node_button))
            {
                cout << "Drawing Node Detected!!!" << endl;
                dragged_node = model.create_node(Node_Type::Drawing, mouse_x, mouse_y);
                dragging = true;
                offset_x = 0;
                offset_y = 0;
                original_x = dragged_node->x;
                original_y = dragged_node->y;
                new_node_dragged = true;
            }
            else if (click_in_rect(mouse_x, mouse_y, peg_node_button))
            {
                cout << "Peg Node Detected!!!" << endl;
                dragged_node = model.create_node(Node_Type::Peg, mouse_x, mouse_y);
                dragging = true;
                offset_x = 0;
                offset_y = 0;
                original_x = dragged_node->x;
                original_y = dragged_node->y;
                new_node_dragged = true;
            }
            else if (click_in_rect(mouse_x, mouse_y, composite_node_button))
            {
                cout << "Composite Node Detected!!!" << endl;
                dragged_node = model.create_node(Node_Type::Composite, mouse_x, mouse_y);
                dragging = true;
                offset_x = 0;
                offset_y = 0;
                original_x = dragged_node->x;
                original_y = dragged_node->y;
                new_node_dragged = true;
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            if (dragging && dragged_node)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                dragged_node->x = mouseX - offset_x;
                dragged_node->y = mouseY - offset_y;
                //cout << "Dragging Detected" << endl;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            if (dragging)
            {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;
                bool overlapping = false;
                SDL_Rect new_node_rect{dragged_node->x, dragged_node->y, 100, 50};
                for (auto node : model.get_nodes())
                {
                    if (node == dragged_node) continue;
                    SDL_Rect other_rect{node->x, node->y, 100, 50};
                    if (rects_overlap(new_node_rect, other_rect))
                    {
                        overlapping = true;
                        break;
                    }
                }
                if (mouse_y > 650 && mouse_y < 950 || overlapping)
                {
                    cout << "Wrong Place!!!" << endl;
                    if (find(model.get_nodes().begin(), model.get_nodes().end(), dragged_node) != model.get_nodes().end() && !new_node_dragged)
                    {
                        dragged_node->x = original_x;
                        dragged_node->y = original_y;
                    }
                    else
                    {
                        model.delete_node(dragged_node);
                    }
                }
                new_node_dragged = false;
                dragging = false;
                dragged_node = nullptr;
                cout << "Mouse Button Release Detected!!!" << endl;
            }
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