/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Summer 2025
 * Date: 28.07.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef H_DRAGON_WARRIORS_H
#define H_DRAGON_WARRIORS_H

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class ArrayMovingObject;
// class DragonWarriorsProgram
class Position;
class MapElement;
class Map;
class Configuration;

class DragonLord;
class SmartDragon;
class Warrior;
class FlyTeam;
class GroundTeam;

class BaseItem;
class DragonScale;
class HealingHerb;
class TrapEnhancer;

class BaseBag;
class TeamBag;

class TestDragonWar;

// ——— Enums ———
enum ElementType
{
    PATH,
    OBSTACLE,
    GROUND_OBSTACLE
};
enum DragonType
{
    SD1,
    SD2,
    SD3
};
enum ItemType
{
    DRAGONSCALE,
    HEALINGHERB,
    TRAPENHANCER
};

// ——— MapElement ———
class MapElement
{
private:
    ElementType type;
    int req_dmg;

public:
    MapElement(ElementType t, int r = 0);

    virtual ~MapElement();
    ElementType getType() const;
    int getReqDmg() const;

    friend class TestDragonWar;
};

// ——— Map ——— need implement deep copy
class Map
{
private:
    int num_rows;
    int num_cols;
    MapElement ***grid;

    void free()
    {
        if (!grid)
            return;
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_cols; ++c)
                delete grid[r][c];
            delete[] grid[r];
        }
        delete[] grid;
        grid = nullptr;
    }

public:
    Map &operator=(const Map &other)
    {
        if (this == &other)
            return *this; // tránh self-assignment

        // 1. Giải phóng dữ liệu cũ
        free();

        // 2. Copy số hàng và số cột
        num_rows = other.num_rows;
        num_cols = other.num_cols;

        // 3. Cấp phát lại mảng 2D grid
        grid = new MapElement **[num_rows];
        for (int r = 0; r < num_rows; ++r)
        {
            grid[r] = new MapElement *[num_cols];
            for (int c = 0; c < num_cols; ++c)
                // Deep copy từng phần tử (đa hình)
                grid[r][c] = new MapElement(other.grid[r][c]->getType(), other.grid[r][c]->getReqDmg());
        }
        return *this;
    }

    Map(int rows, int cols,
        int num_obst, Position *obst,
        int num_gro_obst, Position *gro_obst);

    ~Map();
    // bool isValid(const Position & pos, MovingObject * map) const;
    int getNumRows() const;
    int getNumCols() const;

    bool isValid(const Position &pos, MovingObject *mv_obj) const;

    friend class TestDragonWar;
};

// ——— Position ———
class Position
{
private:
    int r_, c_;

public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position other) const;

    friend class TestDragonWar;
};

// ——— MovingObject ———
class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position &pos, Map *map, const string &name = "")
        : index(index), pos(pos), map(map), name(name) {}

    virtual ~MovingObject() {}

    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const
    {
        return this->pos;
    }
    virtual void move() = 0;
    virtual string str() const = 0;

    friend class TestDragonWar;
    friend class DragonWarriorsProgram;
};

// ——— Warrior ———
class Warrior : public MovingObject
{
protected:
    int hp;
    int damage;
    BaseBag *bag;

public:
    Warrior(int index, const Position &pos, Map *map,
            const string &name, int hp, int damage);
    virtual ~Warrior() {};

    int getHp() const;
    int getDamage() const;
    void setHp(int hp);
    void setDamage(int damage);

    BaseBag *getBag() const;
    friend class TestDragonWar;
};

// ——— FlyTeam & GroundTeam ———
class FlyTeam : public Warrior
{
private:
    static int count;
    string moving_rule;
    int moving_index;

public:
    FlyTeam(int index, const string &moving_rule,
            const Position &pos, Map *map, int hp, int damage);
    ~FlyTeam();

    Position getNextPosition() override;
    void move() override;
    string str() const override;
    // attack()

    friend class TestDragonWar;
};

class GroundTeam : public Warrior
{
private:
    string moving_rule;
    int moving_index;
    int trap_turns;

public:
    GroundTeam(int index, const string &moving_rule,
               const Position &pos, Map *map, int hp, int damage);
    ~GroundTeam();
    // TODO
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    // trap method
    bool trap(DragonLord *dragon);

    // getter/setter
    int getTrapTurns() const;
    void setTrapTurns(int turns);

    friend class TestDragonWar;
};

// ——— DragonLord ———
class DragonLord : public MovingObject
{
private:
    FlyTeam *flyteam1;
    FlyTeam *flyteam2;
    GroundTeam *groundteam;

public:
    int trap_step;
    DragonLord(int index, const Position &pos, Map *map,
               FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *groundteam);

    Position getNextPosition() override;
    void move() override;
    string str() const override;

    friend class TestDragonWar;
};

// need unit test
// ——— ArrayMovingObject ——— need implement deep copy
class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();

    bool isFull() const;
    bool add(MovingObject *mv_obj);
    string str() const;

    friend class TestDragonWar;
    friend class DragonWarriorsProgram;
};

// ——— SmartDragon ———
class SmartDragon : public MovingObject
{
protected:
    DragonType smartdragon_type;
    int damage;
    BaseItem *item;       // SmartDragon sở hữu -> truyền cho Bag thì clone ra -> dragon còn sóng thì tự huỷ
    MovingObject *target; // FlyTeam1 / FlyTeam2 / GroundTeam

    string typeName() const;

public:
    SmartDragon(int index,
                const Position &init_pos,
                Map *map,
                DragonType type,
                MovingObject *target,
                int damage);

    virtual ~SmartDragon(); // nhớ thu hồi item

    int getDmg()
    {
        return damage;
    }

    DragonType getDragonType()
    {
        return smartdragon_type;
    }
    Position getNextPosition() override; // SD1/SD2 rượt theo target; SD3 đứng yên
    void move() override;                // di chuyển nếu hợp lệ
    string str() const override;         // smartdragon[pos=...;type=SDx;tg=...]
    friend class TestDragonWar;
};

// ——— BaseItem ———
class BaseItem
{
public:
    virtual ~BaseItem() = default;
    // virtual BaseItem* clone() const = 0; // clone ảo
    // Kiểm tra điều kiện có thể sử dụng
    virtual bool canUse(Warrior *w) = 0;
    // Áp dụng hiệu ứng của item
    virtual void use(Warrior *w) = 0;
    friend class TestDragonWar;
};

// ——— DragonScale ———
class DragonScale : public BaseItem
{
public:
    DragonScale() = default;
    // DragonScale* clone() const override { return new DragonScale(*this); }
    bool canUse(Warrior *w) override;
    void use(Warrior *w) override;
    friend class TestDragonWar;
};

// ——— HealingHerb ———
class HealingHerb : public BaseItem
{
public:
    HealingHerb() = default;
    // HealingHerb* clone() const override { return new HealingHerb(*this); }
    bool canUse(Warrior *w) override;
    void use(Warrior *w) override;
    friend class TestDragonWar;
};

// ——— TrapEnhancer ———
class TrapEnhancer : public BaseItem
{
public:
    TrapEnhancer() = default;
    // TrapEnhancer* clone() const override { return new TrapEnhancer(*this); }
    bool canUse(Warrior *w) override;
    void use(Warrior *w) override;
    friend class TestDragonWar;
};

// ——— BaseBag ———
class BaseBag
{
protected:
    Warrior *warrior; // đội sở hữu túi
public:
    BaseBag(Warrior *w) : warrior(w) {}
    virtual ~BaseBag() = default;

    // Thêm item vào đầu danh sách
    virtual bool insert(BaseItem *item) = 0;

    // Lấy item đầu tiên có thể dùng
    virtual BaseItem *get() = 0;

    // Lấy item theo loại
    virtual BaseItem *get(ItemType itemType) = 0; // ItemType là enum, dùng int forward

    // Chuỗi thông tin túi
    virtual std::string str() const = 0;
    friend class TestDragonWar;
};

// ——— TeamBag ———
class TeamBag : public BaseBag
{
private:
    struct Node
    {
        BaseItem *item;
        Node *next;
        Node(BaseItem *i, Node *n = nullptr) : item(i), next(n) {}
    };
    Node *head;
    int count;
    int capacity; // 5 cho FlyTeam, 7 cho GroundTeam

    bool isType(BaseItem *item, ItemType type) const
    {
        switch (type)
        {
        case DRAGONSCALE:
            return dynamic_cast<DragonScale *>(item) != nullptr;
        case HEALINGHERB:
            return dynamic_cast<HealingHerb *>(item) != nullptr;
        case TRAPENHANCER:
            return dynamic_cast<TrapEnhancer *>(item) != nullptr;
        default:
            return false;
        }
    }

public:
    TeamBag(Warrior *w);
    ~TeamBag();

    bool insert(BaseItem *item) override;
    BaseItem *get() override;
    BaseItem *get(ItemType itemType) override;
    string str() const override;
    friend class TestDragonWar;
};

// ——— Configuration ———
class Configuration
{
private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;

    int num_obstacles;
    Position *arr_obstacles;

    int num_ground_obstacles;
    Position *arr_ground_obstacles;

    string flyteam1_moving_rule;
    Position flyteam1_init_pos;
    int flyteam1_init_hp;
    int flyteam1_init_damage;

    string flyteam2_moving_rule;
    Position flyteam2_init_pos;
    int flyteam2_init_hp;
    int flyteam2_init_damage;

    string groundteam_moving_rule;
    Position groundteam_init_pos;
    int groundteam_init_hp;
    int groundteam_init_damage;

    Position dragonlord_init_pos;
    int num_steps;

    string removeSpaces(const string &input)
    {
        string result;
        result.reserve(input.size()); // xin trước sức chứa bằng chiều dài input
        for (char ch : input)
        {
            if (ch != ' ')
                result += ch; // chỉ thêm khi không phải khoảng trắng
        }
        return result;
    }

    int countChar(const string &s, char ch)
    {
        int count = 0;
        for (char c : s)
        {
            if (c == ch)
                count++;
        }
        return count;
    }

    Position *handle_arr_pos(string src, int size)
    {
        Position *arr = new Position[(size > 0 ? size : 1)];
        int idx = 0;
        size_t open, close;

        while ((open = src.find('(')) != string::npos)
        {
            close = src.find(')', open);
            if (close == string::npos)
                break; // không tìm thấy dấu đóng

            // Lấy nguyên cụm "(...)" bao gồm cả dấu ngoặc
            string token = src.substr(open, close - open + 1);
            arr[idx++] = Position(token);
            // Xóa cụm đó khỏi chuỗi gốc
            src.erase(open, close - open + 1);
        }
        return arr;
    }

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
    int getFlyTeam1InitHP()
    {
        return this->flyteam1_init_hp;
    }

    friend class TestDragonWar;
    friend class DragonWarriorsProgram;
};

// ——— DragonWarriorsProgram ———
class DragonWarriorsProgram
{
public:
    // Sample attributes

    Configuration *config;

    FlyTeam *flyteam1;
    FlyTeam *flyteam2;
    GroundTeam *groundteam;
    DragonLord *dragonlord;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    DragonWarriorsProgram(const string &config_file_path);

    ~DragonWarriorsProgram();

    bool isStop() const;

    void printResult() const
    {
        Position lord_pos = dragonlord->getCurrentPosition();
        int r = lord_pos.getRow();
        int c = lord_pos.getCol();
        if (flyteam1->getCurrentPosition().isEqual(r, c))
        {
            cout << "FlyTeam1 defeated the DragonLord!" << endl;
        }
        else if (flyteam2->getCurrentPosition().isEqual(r, c))
        {
            cout << "FlyTeam2 defeated the DragonLord!" << endl;
        }
        else
        {
            cout << "The warrior lost the battle!" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << flyteam1->str() << "--|--" << flyteam2->str() << "--|--" << groundteam->str() << "--|--" << dragonlord->str() << endl;
    }

    void run(bool verbose);
};

#endif