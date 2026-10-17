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
    MapElement ***map;

    void free()
    {
        if (!map)
            return;
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_cols; ++c)
                delete map[r][c];
            delete[] map[r];
        }
        delete[] map;
        map = nullptr;
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
        map = new MapElement **[num_rows];
        for (int r = 0; r < num_rows; ++r)
        {
            map[r] = new MapElement *[num_cols];
            for (int c = 0; c < num_cols; ++c)
                // Deep copy từng phần tử (đa hình)
                map[r][c] = new MapElement(other.map[r][c]->getType(), other.map[r][c]->getReqDmg());
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
    virtual void fight(MovingObject *enemy) = 0;

    string getName() const { return name; }
    friend class TestDragonWar;
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
    virtual bool persue(SmartDragon *dragon) = 0;
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
    static const int HISTORY_CAP = 5;
    Position history[HISTORY_CAP];
    int historyCount = 0;

    void pushHistory(const Position &p)
    {
        if (historyCount < HISTORY_CAP)
        {
            history[historyCount++] = p;
        }
        else
        {
            // dịch trái
            for (int i = 0; i < HISTORY_CAP - 1; ++i)
            {
                history[i] = history[i + 1];
            }
            history[HISTORY_CAP - 1] = p;
        }
    }

    void checkTwoPointLoop()
    {
        if (historyCount < 5)
            return;

        Position p0 = history[historyCount - 1]; // mới nhất (A)
        Position p1 = history[historyCount - 2]; // (B)
        Position p2 = history[historyCount - 3]; // (A)
        Position p3 = history[historyCount - 4]; // (B)
        Position p4 = history[historyCount - 5]; // cũ nhất (A)

        bool a_eq_a = p0.isEqual(p2.getRow(), p2.getCol()) &&
                      p2.isEqual(p4.getRow(), p4.getCol());
        bool b_eq_b = p1.isEqual(p3.getRow(), p3.getCol());
        bool a_neq_b = !(p0.isEqual(p1.getRow(), p1.getCol()));

        if (a_eq_a && b_eq_b && a_neq_b)
        {
            cout << "MSG: " << this->name << " eliminated due to being stuck for 3 similar patterns!" << endl;
            this->setHp(1);
        }
    }

public:
    FlyTeam(int index, const string &moving_rule,
            const Position &pos, Map *map, int hp, int damage);
    ~FlyTeam();

    FlyTeam &operator=(const FlyTeam &other);
    void fight(MovingObject *dragon) override;
    bool persue(SmartDragon *dragon) override;
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    bool attack(DragonLord *lord);

    friend class TestDragonWar;
};

class GroundTeam : public Warrior
{
private:
    string moving_rule;
    int moving_index;
    int trap_turns;
    static const int HISTORY_CAP = 5;
    Position history[HISTORY_CAP] = {Position::npos,Position::npos,Position::npos,Position::npos,Position::npos};
    int historyCount = 0;

    void pushHistory(const Position &p)
    {
        if (historyCount < HISTORY_CAP)
        {
            history[historyCount++] = p;
        }
        else
        {
            // dịch trái
            for (int i = 0; i < HISTORY_CAP - 1; ++i)
            {
                history[i] = history[i + 1];
            }
            history[HISTORY_CAP - 1] = p;
        }
    }

    void checkTwoPointLoop()
    {
        if (historyCount < 3)
            return;

        Position p0 = history[historyCount - 1]; // mới nhất (A)
        Position p1 = history[historyCount - 2]; // (B)
        Position p2 = history[historyCount - 3]; // (A)
        Position p3 = history[historyCount - 4]; // (B)
        Position p4 = history[historyCount - 5]; // cũ nhất (A)

        bool a_eq_a = p0.isEqual(p2) &&
                      p2.isEqual(p4);
        bool b_eq_b = p1.isEqual(p3);
        bool a_neq_b = !(p0.isEqual(p1));

        bool conti = p0.isEqual(p1) && p1.isEqual(p2);
        if ((a_eq_a && b_eq_b && a_neq_b) ||  conti)
        {
            cout << "MSG: " << this->name << " eliminated due to being stuck for 3 similar patterns!" << endl;
            this->setHp(1);
        }
    }

public:
    GroundTeam(int index, const string &moving_rule,
               const Position &pos, Map *map, int hp, int damage);
    ~GroundTeam();
    GroundTeam &operator=(const GroundTeam &other);
    void fight(MovingObject *dragon) override;
    bool persue(SmartDragon *dragon) override;

    // TODO
    Position getNextPosition() override;
    void move() override;
    void reverse_trap();
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
    bool hasSD;

public:
    int trap_step;
    bool defeated;
    int moved_step;
    DragonLord(int index, const Position &pos, Map *map,
               FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *groundteam);

    void fight(MovingObject *warrior) override;

    Position getNextPosition() override;
    void move() override;
    string str() const override;
    SmartDragon *creat_smart_dragon(Position prev, int &num_SD1, int &num_SD2, int &num_SD3);

    friend class TestDragonWar;
};

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

    int size() const;
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int idx) const;
    string str() const;

    bool remove(int idx);

    friend class TestDragonWar;
    // friend class DragonWarriorsProgram;
};

// ——— SmartDragon ———
class SmartDragon : public MovingObject
{
protected:
    DragonType smartdragon_type;
    int damage;
    BaseItem *item;       // SmartDragon sở hữu -> truyền cho Bag thì clone ra -> dragon còn sóng thì tự huỷ
    MovingObject *target; // FlyTeam1 / FlyTeam2 / GroundTeam

    FlyTeam *flyteam1;
    FlyTeam *flyteam2;
    GroundTeam *groundteam;

    string typeName() const;

public:
    bool defeated;
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
    void fight(MovingObject *warrior) override;
    bool persued(Warrior *warrior);
    friend class TestDragonWar;
};

// ——— BaseItem ———
class BaseItem
{
public:
    virtual ~BaseItem() = default;
    virtual BaseItem *clone() const = 0; // clone ảo
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
    BaseItem *clone() const override { return new DragonScale(); }
    bool canUse(Warrior *w) override;
    void use(Warrior *w) override;
    friend class TestDragonWar;
};

// ——— HealingHerb ———
class HealingHerb : public BaseItem
{
public:
    HealingHerb() = default;
    BaseItem *clone() const override { return new HealingHerb(); }
    bool canUse(Warrior *w) override;
    void use(Warrior *w) override;
    friend class TestDragonWar;
};

// ——— TrapEnhancer ———
class TrapEnhancer : public BaseItem
{
public:
    TrapEnhancer() = default;
    BaseItem *clone() const override { return new TrapEnhancer(); }
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

    // --- helpers (đặt private trong Configuration) ---
    Position *cloneArray(const Position *src, int n)
    {
        if (!src || n <= 0)
            return nullptr;
        Position *dst = new Position[n];
        for (int i = 0; i < n; ++i)
            dst[i] = src[i];
        return dst;
    }

    void freeArrays()
    {
        delete[] arr_obstacles;
        arr_obstacles = nullptr;
        delete[] arr_ground_obstacles;
        arr_ground_obstacles = nullptr;
    }

public:
    Configuration(const string &filepath);
    ~Configuration();

    Configuration &operator=(const Configuration &o)
    {
        if (this == &o)
            return *this;

        // 1) Copy các field đơn/scalar trước
        map_num_rows = o.map_num_rows;
        map_num_cols = o.map_num_cols;
        max_num_moving_objects = o.max_num_moving_objects;

        flyteam1_moving_rule = o.flyteam1_moving_rule;
        flyteam1_init_pos = o.flyteam1_init_pos;
        flyteam1_init_hp = o.flyteam1_init_hp;
        flyteam1_init_damage = o.flyteam1_init_damage;

        flyteam2_moving_rule = o.flyteam2_moving_rule;
        flyteam2_init_pos = o.flyteam2_init_pos;
        flyteam2_init_hp = o.flyteam2_init_hp;
        flyteam2_init_damage = o.flyteam2_init_damage;

        groundteam_moving_rule = o.groundteam_moving_rule;
        groundteam_init_pos = o.groundteam_init_pos;
        groundteam_init_hp = o.groundteam_init_hp;
        groundteam_init_damage = o.groundteam_init_damage;

        dragonlord_init_pos = o.dragonlord_init_pos;
        num_steps = o.num_steps;

        // 2) Deep copy hai mảng động
        //    - Giải phóng cũ
        delete[] arr_obstacles;
        arr_obstacles = nullptr;
        delete[] arr_ground_obstacles;
        arr_ground_obstacles = nullptr;

        //    - Sao chép số lượng
        num_obstacles = o.num_obstacles;
        num_ground_obstacles = o.num_ground_obstacles;

        //    - Cấp phát & chép dữ liệu
        arr_obstacles = cloneArray(o.arr_obstacles, num_obstacles);
        arr_ground_obstacles = cloneArray(o.arr_ground_obstacles, num_ground_obstacles);

        return *this;
    }

    string str() const;
    int getFlyTeam1InitHP()
    {
        return this->flyteam1_init_hp;
    }

    // Getter methods for Configuration

    int getMapNumRows() const { return map_num_rows; }
    int getMapNumCols() const { return map_num_cols; }
    int getMaxNumMovingObjects() const { return max_num_moving_objects; }

    int getNumObstacles() const { return num_obstacles; }
    Position *getArrObstacles() const { return arr_obstacles; }

    int getNumGroundObstacles() const { return num_ground_obstacles; }
    Position *getArrGroundObstacles() const { return arr_ground_obstacles; }

    string getFlyTeam1MovingRule() const { return flyteam1_moving_rule; }
    Position getFlyTeam1InitPos() const { return flyteam1_init_pos; }
    int getFlyTeam1InitHP() const { return flyteam1_init_hp; }
    int getFlyTeam1InitDamage() const { return flyteam1_init_damage; }

    string getFlyTeam2MovingRule() const { return flyteam2_moving_rule; }
    Position getFlyTeam2InitPos() const { return flyteam2_init_pos; }
    int getFlyTeam2InitHP() const { return flyteam2_init_hp; }
    int getFlyTeam2InitDamage() const { return flyteam2_init_damage; }

    string getGroundTeamMovingRule() const { return groundteam_moving_rule; }
    Position getGroundTeamInitPos() const { return groundteam_init_pos; }
    int getGroundTeamInitHP() const { return groundteam_init_hp; }
    int getGroundTeamInitDamage() const { return groundteam_init_damage; }

    Position getDragonLordInitPos() const { return dragonlord_init_pos; }
    int getNumSteps() const { return num_steps; }

    // Setter methods for Configuration

    void setMapNumRows(int val) { map_num_rows = val; }
    void setMapNumCols(int val) { map_num_cols = val; }
    void setMaxNumMovingObjects(int val) { max_num_moving_objects = val; }

    void setNumObstacles(int val) { num_obstacles = val; }
    void setArrObstacles(Position *arr)
    {
        // giải phóng cũ
        delete[] arr_obstacles;
        arr_obstacles = nullptr;

        if (arr && num_obstacles > 0)
        {
            arr_obstacles = cloneArray(arr, num_obstacles);
        }
    }

    void setNumGroundObstacles(int val) { num_ground_obstacles = val; }
    void setArrGroundObstacles(Position *arr)
    {
        delete[] arr_ground_obstacles;
        arr_ground_obstacles = nullptr;

        if (arr && num_ground_obstacles > 0)
        {
            arr_ground_obstacles = cloneArray(arr, num_ground_obstacles);
        }
    }

    // --- sửa setter: deep copy dựa trên số lượng ---
    // Obstacle
    // void setArrObstacles(const Position *arr, int n)
    // {
    //     // giải phóng cũ
    //     delete[] arr_obstacles;
    //     arr_obstacles = nullptr;
    //     num_obstacles = 0;
    //     if (arr && n > 0)
    //     {
    //         arr_obstacles = cloneArray(arr, n);
    //         num_obstacles = n;
    //     }
    // }
    // Ground obstacle
    // void setArrGroundObstacles(const Position *arr, int n)
    // {
    //     delete[] arr_ground_obstacles;
    //     arr_ground_obstacles = nullptr;
    //     num_ground_obstacles = 0;
    //     if (arr && n > 0)
    //     {
    //         arr_ground_obstacles = cloneArray(arr, n);
    //         num_ground_obstacles = n;
    //     }
    // }

    void setFlyTeam1MovingRule(const string &val) { flyteam1_moving_rule = val; }
    void setFlyTeam1InitPos(const Position &pos) { flyteam1_init_pos = pos; }
    void setFlyTeam1InitHP(int hp) { flyteam1_init_hp = hp; }
    void setFlyTeam1InitDamage(int dmg) { flyteam1_init_damage = dmg; }

    void setFlyTeam2MovingRule(const string &val) { flyteam2_moving_rule = val; }
    void setFlyTeam2InitPos(const Position &pos) { flyteam2_init_pos = pos; }
    void setFlyTeam2InitHP(int hp) { flyteam2_init_hp = hp; }
    void setFlyTeam2InitDamage(int dmg) { flyteam2_init_damage = dmg; }

    void setGroundTeamMovingRule(const string &val) { groundteam_moving_rule = val; }
    void setGroundTeamInitPos(const Position &pos) { groundteam_init_pos = pos; }
    void setGroundTeamInitHP(int hp) { groundteam_init_hp = hp; }
    void setGroundTeamInitDamage(int dmg) { groundteam_init_damage = dmg; }

    void setDragonLordInitPos(const Position &pos) { dragonlord_init_pos = pos; }
    void setNumSteps(int val) { num_steps = val; }

    friend class TestDragonWar;
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

    friend class TestDragonWar;
};

#endif