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
class Node;
class Wire;

struct way_point
{
    int x, y;
};


class Node
{
    int active_input_count = 0;
    int active_output_count = 10;
public:
    int x, y;
    Node_Type type;

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
    int get_active_input_count() const {return active_input_count;}
    int get_active_output_count() const {return active_output_count;}
    void use_input() {active_input_count--;}
    void use_output() {active_output_count--;}
};

struct Wire
{
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    vector<way_point> way_points;

    Wire(int fx, int fy, int tx, int ty) : from_x(fx), from_y(fy), to_x(tx), to_y(ty) {}
};


class Model
{
private:
    vector<Node*> nodes;
    vector<Wire*> wires;
    int temp_cursor_x;
    int temp_cursor_y;
    Node* wiring_node;
    bool wiring_mode = false;
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

    Wire* create_wire(int from_x, int from_y, int to_x, int to_y)
    {
        Wire* wire = new Wire(from_x, from_y, to_x, to_y);
        wires.push_back(wire);
        return wire;
    }

    void cursor_pos_wiring(int cx, int cy)
    {
        temp_cursor_x = cx;
        temp_cursor_y = cy;
    }

    int get_wiring_x() const{return temp_cursor_x;}
    int get_wiring_y() const{return temp_cursor_y;}

    void set_wiring_start_node(Node* node)
    {
        wiring_node = node;
    }
    Node* get_wiring_start_node() const{return wiring_node;}

    void delete_node(Node* node)
    {
        nodes.erase(remove(nodes.begin(), nodes.end(), node), nodes.end());
        delete node;
    }
    void switch_wiring_mode(bool stat)
    {
        wiring_mode = stat;
    }
    bool is_wiring_mode() const{return wiring_mode;}

    const vector<Node*>& get_nodes() const { return nodes; }
    const vector<Wire*>& get_wires() const { return wires; }
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
        render_wires(model);
        if (model.is_wiring_mode())
        {
            draw_temp_wire(model);
        }
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
        int start_x = model.get_wiring_start_node()->x + (10 - model.get_wiring_start_node()->get_active_output_count()) * 10;
        int start_y = model.get_wiring_start_node()->y + 50;
        int end_x = model.get_wiring_x();
        int end_y = model.get_wiring_y();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
    }
    void render_wires(const Model& model)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& wire: model.get_wires())
        {
        // Notice: this part is Completely copied from CHAT_GPT!!!
            float x0 = wire->from_x;
            float y0 = wire->from_y;
            float x1 = wire->to_x;
            float y1 = wire->to_y;

            float cx0 = x0 + (x1 - x0) / 2;
            float cy0 = y0;
            float cx1 = x0 + (x1 - x0) / 2;
            float cy1 = y1;

            const int STEPS = 30;
            for (int i = 0; i < STEPS; ++i)
            {
                float t1 = i / (float)STEPS;
                float t2 = (i + 1) / (float)STEPS;

                float xa = (1 - t1) * (1 - t1) * (1 - t1) * x0 + 3 * (1 - t1) * (1 - t1) * t1 * cx0 + 3 * (1 - t1) * t1 * t1 * cx1 + t1 * t1 * t1 * x1;
                float ya = (1 - t1) * (1 - t1) * (1 - t1) * y0 + 3 * (1 - t1) * (1 - t1) * t1 * cy0 + 3 * (1 - t1) * t1 * t1 * cy1 + t1 * t1 * t1 * y1;

                float xb = (1 - t2) * (1 - t2) * (1 - t2) * x0 + 3 * (1 - t2) * (1 - t2) * t2 * cx0 + 3 * (1 - t2) * t2 * t2 * cx1 + t2 * t2 * t2 * x1;
                float yb = (1 - t2) * (1 - t2) * (1 - t2) * y0 + 3 * (1 - t2) * (1 - t2) * t2 * cy0 + 3 * (1 - t2) * t2 * t2 * cy1 + t2 * t2 * t2 * y1;

                SDL_RenderDrawLine(renderer, (int)xa, (int)ya, (int)xb, (int)yb);
            }
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
                    model.set_wiring_start_node(start_wire_node);
                    model.switch_wiring_mode(true);
                }
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            if (dragging && dragged_node)
            {
                dragged_node->x = mouseX - offset_x;
                dragged_node->y = mouseY - offset_y;
                //cout << "Dragging Detected" << endl;
            }
            else if (wiring_mode)
            {
                model.cursor_pos_wiring(mouseX, mouseY);
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
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
        {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;
            for (auto& node: model.get_nodes())
            {
                SDL_Rect current_node{node->x, node->y, 100, 50};
                if (click_in_rect(mouse_x, mouse_y, current_node) && model.get_wiring_start_node()->get_active_output_count() != 0)
                {
                    model.create_wire(model.get_wiring_start_node()->x + (10 - model.get_wiring_start_node()->get_active_output_count()) * 10, model.get_wiring_start_node()->y + 50, node->x + 50, node->y);
                    cout << "Wired Up!!!" << endl;
                    model.get_wiring_start_node()->use_output();
                    node->use_input();
                }
            }
            if (wiring_mode)
            {
                wiring_mode = false;
                model.switch_wiring_mode(false);
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