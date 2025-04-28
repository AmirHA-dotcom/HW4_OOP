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
    Port input_ports[10];
    Port output_ports[10];
    int active_input_count = 0;
    int active_output_count = 10;

    Node(Node_Type t, int posX, int posY) : type(t), x(posX), y(posY)
    {
        if (type == Node_Type::Drawing || type == Node_Type::Peg)
        {
            active_input_count = 1;
        }
        else if (type == Node_Type::Composite)
        {
            active_input_count = 10;
        }
    }

    void add_output_port()
    {
        if (active_output_count < 10)
            active_output_count++;
    }

    void add_input_port()
    {
        if (type == Node_Type::Composite && active_input_count < 10)
            active_input_count++;
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
    Port* temp_wire_start = nullptr;  // From which output port we are starting

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

    void draw_wire_from_start_node_to_cursor(Node* node, int mouse_x, int mouse_y)
    {
        if (!node) return;

        temp_wire_start = &node->output_ports[0];

        static Port cursor_port;
        cursor_port.node = nullptr;
        cursor_port.type = Port_Type::Input;
        cursor_port.index = -1;

        Wire temp_wire(temp_wire_start, &cursor_port);

        way_point p1 = { node->x, node->y };
        way_point p2 = { mouse_x, mouse_y };
        temp_wire.way_points.push_back(p1);
        temp_wire.way_points.push_back(p2);
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
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
        SDL_Rect Drawing_Node{400, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Drawing_Node);
        SDL_RenderFillRect(renderer, &Drawing_Node);
        SDL_Color textColor2 = {255, 255, 255};
        const char* text2 = "Drawing";
        SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, text2, textColor2);
        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        int text2W, text2H;
        SDL_QueryTexture(textTexture2, nullptr, nullptr, &text2W, &text2H);
        SDL_Rect textRect2{Drawing_Node.x + (Drawing_Node.w - text2W) / 2,Drawing_Node.y + (Drawing_Node.h - text2H) / 2,text2W,text2H};
        SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 400, 750, 550, 750);
        SDL_RenderDrawLine(renderer, 400, 900, 550, 900);
        SDL_RenderDrawLine(renderer, 400, 750, 400, 900);
        SDL_RenderDrawLine(renderer, 550, 750, 550, 900);

// Peg Node Icon
        SDL_SetRenderDrawColor(renderer, 0, 168, 107, 255);
        SDL_Rect Peg_Node{700, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Peg_Node);
        SDL_Color textColor3 = {255, 255, 255};
        const char* text3 = "Peg Node";
        SDL_Surface* textSurface3 = TTF_RenderText_Blended(font, text3, textColor3);
        SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
        int text3W, text3H;
        SDL_QueryTexture(textTexture3, nullptr, nullptr, &text3W, &text3H);
        SDL_Rect textRect3{Peg_Node.x + (Peg_Node.w - text3W) / 2,Peg_Node.y + (Peg_Node.h - text3H) / 2,text3W,text3H};
        SDL_RenderCopy(renderer, textTexture3, nullptr, &textRect3);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 700, 750, 850, 750);
        SDL_RenderDrawLine(renderer, 700, 900, 850, 900);
        SDL_RenderDrawLine(renderer, 700, 750, 700, 900);
        SDL_RenderDrawLine(renderer, 850, 750, 850, 900);

// Composite Node Icon
        SDL_SetRenderDrawColor(renderer, 17, 30, 108, 255);
        SDL_Rect Composite_Node{1000, 750, 150, 150};
        SDL_RenderFillRect(renderer, &Composite_Node);
        SDL_Color textColor4 = {255, 255, 255};
        const char* text4 = "Composite";
        SDL_Surface* textSurface4 = TTF_RenderText_Blended(font, text4, textColor4);
        SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);
        int text4W, text4H;
        SDL_QueryTexture(textTexture4, nullptr, nullptr, &text4W, &text4H);
        SDL_Rect textRect4{Composite_Node.x + (Composite_Node.w - text4W) / 2,Composite_Node.y + (Composite_Node.h - text4H) / 2,text4W,text4H};
        SDL_RenderCopy(renderer, textTexture4, nullptr, &textRect4);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawLine(renderer, 1000, 750, 1150, 750);
        SDL_RenderDrawLine(renderer, 1000, 900, 1150, 900);
        SDL_RenderDrawLine(renderer, 1000, 750, 1000, 900);
        SDL_RenderDrawLine(renderer, 1150, 750, 1150, 900);

// Side Bar
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect side_bar{0, 0, 50, 700};
        SDL_RenderFillRect(renderer, &side_bar);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, 50, 0, 50, 700);
        SDL_RenderDrawLine(renderer, 0, 700, 50, 700);
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_Rect deleter{15, 15, 20, 20};
        SDL_RenderFillRect(renderer, &deleter);


        render_nodes(model);

        SDL_RenderPresent(renderer);
    }
    void render_nodes(const Model& model)
    {
        for (auto node : model.get_nodes())
        {
            SDL_Rect rect = {node->x, node->y, 100, 50};
            if (node->type == Node_Type::Drawing)
                SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
            else if (node->type == Node_Type::Peg)
                SDL_SetRenderDrawColor(renderer, 0, 168, 107, 255);
            else if (node->type == Node_Type::Composite)
                SDL_SetRenderDrawColor(renderer, 17, 30, 108, 255);

            SDL_RenderFillRect(renderer, &rect);
        }
    }
    void draw_temp_wire(const Model& model)
    {

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
    bool new_node_dragged;
    bool delete_mode;
    Node* start_wire_node;
    Node* end_wire_node;
    bool wiring_mode;

public:
    Controller(View& v, Model& m) : view(v), model(m)
    {
        dragging = false; dragged_node = nullptr; offset_x = 0; offset_y = 0; original_x = 0; original_y = 0; new_node_dragged = false; delete_mode = false;
        start_wire_node = nullptr; end_wire_node = nullptr; wiring_mode = false;
    }

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
            SDL_Rect deleter{15, 15, 20, 20};
            for (auto node : model.get_nodes())
            {
                SDL_Rect node_rect{node->x, node->y, 100, 50};
                if (click_in_rect(mouse_x, mouse_y, node_rect))
                {
                    if (delete_mode)
                    {
                        model.delete_node(node);
                        break;
                    }
                    dragged_node = node;
                    dragging = true;
                    offset_x = mouse_x - node->x;
                    offset_y = mouse_y - node->y;
                    original_x = node->x;
                    original_y = node->y;
                    break;
                }
            }
            delete_mode = false;
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
            else if (click_in_rect(mouse_x, mouse_y, deleter))
            {
                delete_mode = true;
                cout << "Deleter Detected!!!" << endl;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
        {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            for (auto& node:model.get_nodes())
            {
                SDL_Rect current_node{node->x, node->y, 100, 50};
                if (click_in_rect(mouse_x, mouse_y, current_node))
                {
                    wiring_mode = true;
                    start_wire_node = node;
                }
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
                SDL_Rect side_bar{0, 0, 50, 700};
                if ((mouse_y > 650 && mouse_y < 950) || overlapping || (rects_overlap(side_bar, new_node_rect)))
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