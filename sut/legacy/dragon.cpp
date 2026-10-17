#include "dragon.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Map element
MapElement::MapElement(ElementType t, int r) : type(t), req_dmg(r) {}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    return this->type;
}

int MapElement::getReqDmg() const
{
    return this->req_dmg;
}

// Map
Map::Map(int rows, int cols, int num_obst, Position *obst, int num_gro_obst, Position *gro_obst)
{
    this->num_rows = rows;
    this->num_cols = cols;

    map = new MapElement **[num_rows];
    for (int r = 0; r < num_rows; ++r)
        map[r] = new MapElement *[num_cols];

    // Duyệt qua từng ô trong bản đồ
    for (int r = 0; r < num_rows; ++r)
    {
        for (int c = 0; c < num_cols; ++c)
        {

            // Mặc định là PATH
            ElementType type = PATH;

            // Kiểm tra có nằm trong OBSTACLE không
            for (int i = 0; i < num_obst; ++i)
            {
                if (obst[i].isEqual(r, c))
                {
                    type = OBSTACLE;
                    break;
                }
            }

            // Nếu không phải OBSTACLE thì kiểm tra GROUND_OBSTACLE
            if (type == PATH)
            {
                for (int i = 0; i < num_gro_obst; ++i)
                {
                    if (gro_obst[i].isEqual(r, c))
                    {
                        type = GROUND_OBSTACLE;
                        break;
                    }
                }
            }

            // Tạo MapElement tương ứng
            if (type == GROUND_OBSTACLE)
            {
                int req_dmg = (r * 257 + c * 139 + 89) % 900 + 1;
                map[r][c] = new MapElement(GROUND_OBSTACLE, req_dmg);
            }
            else
                map[r][c] = new MapElement(type); // OBSTACLE hoặc PATH
        }
    }
}

Map::~Map()
{
    free();
}

int Map::getNumRows() const
{
    return num_rows;
}

int Map::getNumCols() const
{
    return num_cols;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int r = pos.getRow();
    int c = pos.getCol();

    // Ngoài biên bản đồ
    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols)
        return false;

    if (dynamic_cast<DragonLord *>(mv_obj))
        return true;

    MapElement *element = map[r][c];
    ElementType type = element->getType();

    // PATH: luôn đi được
    if (type == PATH)
        return true;

    // OBSTACLE: không thể đi
    if (type == OBSTACLE)
        return false;

    // GROUND_OBSTACLE:
    // Nếu là GroundTeam → phải xét damage
    GroundTeam *gt = dynamic_cast<GroundTeam *>(mv_obj);
    if (!gt)
        return true;
    else
    {
        if (gt->getDamage() > element->getReqDmg())
            return true;
        else
            return false;
    }

    return true;
}

// Positon
const Position Position::npos = Position(-1, -1);

Position::Position(int r, int c) : r_(r), c_(c) {}

Position::Position(const string &str_pos)
{
    int r, c;
    char ch;

    // Tạo bản sao chuỗi và loại bỏ khoảng trắng
    string cleaned;
    for (char ch : str_pos)
    {
        if (ch != ' ')
            cleaned += ch;
    }

    // Format chuẩn sau khi loại bỏ khoảng trắng: "(a,b)"
    stringstream ss(cleaned);
    ss >> ch >> r >> ch >> c >> ch;

    r_ = r;
    c_ = c;
}

int Position::getRow() const
{
    return r_;
}

int Position::getCol() const
{
    return c_;
}

void Position::setRow(int r)
{
    r_ = r;
}

void Position::setCol(int c)
{
    c_ = c;
}

string Position::str() const
{
    return "(" + to_string(r_) + "," + to_string(c_) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{
    return (this->r_ == in_r) && (c_ == in_c);
}

bool Position::isEqual(Position other) const
{
    int in_r = other.getRow();
    int in_c = other.getCol();
    return (r_ == in_r) && (c_ == in_c);
}

// Warrior
Warrior::Warrior(int index, const Position &pos, Map *map, const string &name, int hp, int damage)
    : MovingObject(index, pos, map, name) // gọi constructor lớp cha
{
    // Clamp HP về [0, 500]
    if (hp < 0)
        this->hp = 0;
    else if (hp > 500)
        this->hp = 500;
    else
        this->hp = hp;

    // Clamp Damage về [0, 900]
    if (damage < 0)
        this->damage = 0;
    else if (damage > 900)
        this->damage = 900;
    else
        this->damage = damage;

    // Bag
    this->bag = nullptr;
}

int Warrior::getHp() const
{
    return hp;
}

int Warrior::getDamage() const
{
    return damage;
}

void Warrior::setHp(int hp)
{
    if (hp <= 1)
    {
        this->hp = 0;
        this->pos = Position::npos;
    }
    else if (hp > 500)
        this->hp = 500;
    else
        this->hp = hp;
}

void Warrior::setDamage(int damage)
{
    if (damage < 0)
        this->damage = 0;
    else if (damage > 900)
        this->damage = 900;
    else
        this->damage = damage;
}

BaseBag *Warrior::getBag() const
{
    return this->bag;
}

// FlyTeam
int FlyTeam::count = 0; // khởi tạo giá trị ban đầu

FlyTeam::FlyTeam(int index, const string &moving_rule, const Position &pos,
                 Map *map, int init_hp, int init_damage)
    : Warrior(index, pos, map, "", init_hp, init_damage) // gọi constructor Warrior ? param name
{
    name = "FlyTeam" + to_string(++count);
    this->moving_rule = moving_rule;
    this->moving_index = 0; // bắt đầu từ ký tự đầu tiên trong moving_rule ? moving index
    this->bag = new TeamBag(this);
    this->pushHistory(pos);
}

FlyTeam::~FlyTeam()
{
    count--;
    // cout << count << endl;
    delete bag;
}

FlyTeam &FlyTeam::operator=(const FlyTeam &other)
{
    if (this == &other)
        return *this;

    // Copy phần base class
    this->index = other.index;
    this->pos = other.pos;
    this->map = other.map;
    this->name = other.name;
    this->hp = other.hp;
    this->damage = other.damage;

    // Copy moving_rule và moving_index
    this->moving_rule = other.moving_rule;
    this->moving_index = other.moving_index;

    // Tạo lại bag mới rỗng
    if (this->bag)
        delete this->bag;
    this->bag = new TeamBag(this);

    return *this;
}

Position FlyTeam::getNextPosition()
{
    if (moving_index >= moving_rule.length())
        return Position::npos;

    // Lấy vị trí hiện tại
    int r = pos.getRow();
    int c = pos.getCol();

    char dir = moving_rule[moving_index];

    auto tryMove = [&](char d) -> Position
    {
        int new_r = r;
        int new_c = c;
        switch (d)
        {
        case 'L':
            new_c--;
            break;
        case 'R':
            new_c++;
            break;
        case 'U':
            new_r--;
            break;
        case 'D':
            new_r++;
            break;
        default:
            return Position::npos;
        }
        Position next_pos(new_r, new_c);
        return next_pos;
    };

    // Thử hướng ban đầu
    Position next_pos = tryMove(dir);
    if (!next_pos.isEqual(Position::npos) && map->isValid(next_pos, this))
        return next_pos;

    cout << "MSG: " << this->getName() << " at " << this->getCurrentPosition().str() << " got blocked when moving " << dir << " to " << next_pos.str() << endl;
    // Đảo hướng và thử lại
    char reverse_dir;
    switch (dir)
    {
    case 'L':
        reverse_dir = 'R';
        break;
    case 'R':
        reverse_dir = 'L';
        break;
    case 'U':
        reverse_dir = 'D';
        break;
    case 'D':
        reverse_dir = 'U';
        break;
    }

    next_pos = tryMove(reverse_dir);

    cout << "MSG: " << this->getName() << " moved to the opposite direction " << reverse_dir << " to " << next_pos.str() << endl;
    if (!next_pos.isEqual(Position::npos) && map->isValid(next_pos, this))
    {
        return next_pos;
    }

    cout << "MSG: " << this->getName() << " got blocked both side, stay at " << this->getCurrentPosition().str() << endl;

    // Cả hai hướng đều không hợp lệ
    return Position::npos;
}

void FlyTeam::move()
{
    if (this->hp <= 1)
        return;
    int len = this->moving_rule.length();

    cout << "MSG: " << this->name << " moved" << endl;
    for (int i = 0; i < len; i++)
    {
        Position next = getNextPosition();
        // Nếu có bước đi hợp lệ thì cập nhật vị trí
        if (!next.isEqual(Position::npos))
        {
            pos = next;
            moving_index++;
        }
    }
    pushHistory(pos);
    checkTwoPointLoop();
    moving_index = 0;
}

string FlyTeam::str() const
{
    return this->name +
           "[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";moving_rule=" + moving_rule + "]";
}

bool FlyTeam::attack(DragonLord *lord)
{

    if (lord->getCurrentPosition().isEqual(this->pos))
    {
        cout << "MSG: " << this->getName() << " encounters " << lord->getName() << endl;
        return lord->defeated = true;
    }
    return false;
}

void FlyTeam::fight(MovingObject *dragon)
{
    if (!dragon->getCurrentPosition().isEqual(this->pos))
        return;

    if (dragon == this)
        return;

    if (this->hp <= 1)
        return;

    while (auto item = this->bag->get())
    {
        item->use(this);
    }

    // Nếu là SmartDragon
    if (auto sdra = dynamic_cast<SmartDragon *>(dragon))
    {
        cout << "MSG: " << this->getName() << " encounters " << dragon->getName() << endl;
        this->persue(sdra);
    }
    else if (auto lord = dynamic_cast<DragonLord *>(dragon))
    {
        this->attack(lord);
    }
    else if (auto fly = dynamic_cast<FlyTeam *>(dragon))
    {
        cout << "MSG: " << this->getName() << " meets " << fly->getName() << endl;
    }
    else if (auto ground = dynamic_cast<GroundTeam *>(dragon))
    {
        cout << "MSG: " << this->getName() << " meets " << ground->getName() << endl;
    }

    while (auto item = this->bag->get())
    {
        item->use(this);
    }
    // Nếu không thuộc các loại trên
    // TODO: xử lý mặc định
}

bool FlyTeam::persue(SmartDragon *dragon)
{
    if (!dragon->getCurrentPosition().isEqual(this->pos))
        return false;

    if (this->damage >= dragon->getDmg())
    {
        dragon->defeated = true;
        BaseItem *item_collected;
        bool isteam1 = (this->name.find('1') != string::npos ? true : false);
        switch (dragon->getDragonType())
        {
        case SD1:
            item_collected = new DragonScale();
            break;
        case SD2:
            item_collected = new HealingHerb();
            break;
        case SD3:
            if (isteam1)
                item_collected = new HealingHerb();
            else
                item_collected = new DragonScale();
            break;
        }

        if (item_collected->canUse(this))
        {
            item_collected->use(this);
            delete item_collected;
        }
        else
            this->getBag()->insert(item_collected);

        return true;
    }

    this->hp -= 100;
    return false;
}

// GroundTeam
GroundTeam::GroundTeam(int index, const string &moving_rule, const Position &pos,
                       Map *map, int init_hp, int init_damage)
    : Warrior(index, pos, map, "GroundTeam", init_hp, init_damage)
{
    this->moving_rule = moving_rule;
    this->moving_index = 0; // bắt đầu từ ký tự đầu tiên trong moving_rule ? moving index
    this->trap_turns = 3;
    this->bag = new TeamBag(this);
    this->pushHistory(pos);
}

GroundTeam::~GroundTeam()
{
    delete bag;
}

GroundTeam &GroundTeam::operator=(const GroundTeam &other)
{
    if (this == &other)
        return *this;

    // Copy phần base class
    this->index = other.index;
    this->pos = other.pos;
    this->map = other.map;
    this->name = other.name;
    this->hp = other.hp;
    this->damage = other.damage;

    // Copy moving_rule và moving_index
    this->moving_rule = other.moving_rule;
    this->moving_index = other.moving_index;
    this->trap_turns = other.trap_turns;
    // Tạo lại bag mới rỗng
    if (this->bag)
        delete this->bag;
    this->bag = new TeamBag(this);

    return *this;
}

Position GroundTeam::getNextPosition()
{
    if (moving_index >= moving_rule.length())
        return Position::npos;

    // Lấy vị trí hiện tại
    int r = pos.getRow();
    int c = pos.getCol();

    char dir = moving_rule[moving_index];

    auto tryMove = [&](char d) -> Position
    {
        int new_r = r;
        int new_c = c;
        switch (d)
        {
        case 'L':
            new_c--;
            break;
        case 'R':
            new_c++;
            break;
        case 'U':
            new_r--;
            break;
        case 'D':
            new_r++;
            break;
        default:
            return Position::npos;
        }
        Position next_pos(new_r, new_c);
        if (map)
            return next_pos;
        return Position::npos;
    };

    // Thử hướng ban đầu
    Position next_pos = tryMove(dir);
    if (!next_pos.isEqual(Position::npos) && map->isValid(next_pos, this))
        return next_pos;

    cout << "MSG: " << this->getName() << " at " << this->getCurrentPosition().str() << " got blocked when moving " << dir << " to " << next_pos.str() << endl;
    // Đảo hướng và thử lại
    // Đảo hướng và thử lại
    char reverse_dir;
    switch (dir)
    {
    case 'L':
        reverse_dir = 'R';
        break;
    case 'R':
        reverse_dir = 'L';
        break;
    case 'U':
        reverse_dir = 'D';
        break;
    case 'D':
        reverse_dir = 'U';
        break;
    default:
        return Position::npos;
    }

    next_pos = tryMove(reverse_dir);
    cout << "MSG: " << this->getName() << " moved to the opposite direction " << reverse_dir << " to " << next_pos.str() << endl;

    if (!next_pos.isEqual(Position::npos) && map->isValid(next_pos, this))
        return next_pos;

    cout << "MSG: " << this->getName() << " got blocked both side, stay at " << this->getCurrentPosition().str() << endl;
    // Cả hai hướng đều không hợp lệ
    return Position::npos;
}

void GroundTeam::move()
{
    if (this->hp <= 1)
        return;
    int len = this->moving_rule.length();

    cout << "MSG: " << this->name << " moved" << endl;
    for (int i = 0; i < len; i++)
    {
        Position next = getNextPosition();
        // Nếu có bước đi hợp lệ thì cập nhật vị trí
        if (!next.isEqual(Position::npos))
        {
            pos = next;
            moving_index++;
        }
    }
    pushHistory(pos);
    checkTwoPointLoop();
    moving_index = 0;
}

void GroundTeam::reverse_trap()
{
    int r = pos.getRow();
    int c = pos.getCol();
    swap(r, c);

    if (r >= this->map->getNumRows())
        r = this->map->getNumRows();
    if (c >= this->map->getNumCols())
        c = this->map->getNumCols();

    Position next_pos(r, c);

    if (hp <= 1)
        next_pos = Position::npos;
    // 2) Nếu ô đảo hợp lệ -> di chuyển
    if (this->map && this->map->isValid(next_pos, this))
    {
        pos = next_pos;
        cout << "MSG: DragonLord escaped the trap! GroundTeam transferred to " << pos.str() << endl;
        return;
    }

    // 3) Không hợp lệ -> thử lần lượt URDL (isValid sẽ tự kiểm tra biên)
    const int dr[4] = {-1, 0, 1, 0}; // U R D L
    const int dc[4] = {0, 1, 0, -1};

    for (int k = 0; k < 4; ++k)
    {
        Position cand(r + dr[k], c + dc[k]);
        if (this->map && this->map->isValid(cand, this))
        {
            pos = cand;
            cout << "MSG: DragonLord escaped the trap! GroundTeam transfer to position " << pos.str() << endl;
            return;
        }
    }

    // 4) Không hướng nào nhận -> set máu về 1
    cout << "MSG: DragonLord escaped the trap! Failed to move GroundTeam - eliminated!" << endl;
    this->hp = 1; // hoặc setHP(1) nếu bạn có setter
}

string GroundTeam::str() const
{
    return "GroundTeam[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";moving_rule=" + moving_rule + "]";
}

bool GroundTeam::trap(DragonLord *dragon)
{
    if (dragon->trap_step > 0)
        return false;
    int r = dragon->getCurrentPosition().getRow();
    int c = dragon->getCurrentPosition().getCol();
    if (this->getCurrentPosition().isEqual(r, c))
    {
        
        dragon->trap_step = this->trap_turns;
        cout << "MSG: GroundTeam trapped DragonLord for " << this->trap_turns << " turns" << endl;
        this->trap_turns = 3;
        return true;
    }
    return false;
}

int GroundTeam::getTrapTurns() const
{
    return trap_turns;
}

void GroundTeam::setTrapTurns(int turns)
{
    trap_turns = turns;
}

void GroundTeam::fight(MovingObject *dragon)
{
    if (!dragon->getCurrentPosition().isEqual(this->pos))
        return;

    if (dragon == this)
        return;

    if (this->hp <= 1)
        return;
    // Nếu là SmartDragon

    while (auto item = this->bag->get())
        item->use(this);

    if (auto sdra = dynamic_cast<SmartDragon *>(dragon))
    {
        cout << "MSG: " << this->getName() << " encounters " << sdra->getName() << endl;
        this->persue(sdra);
    }
    // Nếu là DragonLord
    else if (auto lord = dynamic_cast<DragonLord *>(dragon))
    {
        // TODO: xử lý trap hoặc bị teleport theo đề bài
        cout << "MSG: " << this->getName() << " encounters " << lord->getName() << endl;
        this->trap(lord);
    }
    else if (auto fly = dynamic_cast<FlyTeam *>(dragon))
    {
        cout << "MSG: " << this->getName() << " meets " << fly->getName() << endl;
    }

    else if (auto ground = dynamic_cast<GroundTeam *>(dragon))
    {
        cout << "MSG: " << this->getName() << " meets " << ground->getName() << endl;
    }

    while (auto item = this->bag->get())
        item->use(this);
    // Nếu không thuộc các loại trên
    // TODO: xử lý mặc định
}

bool GroundTeam::persue(SmartDragon *dragon)
{

    if (!dragon->getCurrentPosition().isEqual(this->pos))
        return false;

    // cout << "MSG: " << this->getName() << " encounters " << dragon->getName() << endl;
    if (this->damage >= dragon->getDmg())
    {
        dragon->defeated = true;
        BaseItem *item_collected;

        switch (dragon->getDragonType())
        {
        case SD1:
            item_collected = new DragonScale();
            break;
        case SD2:
            item_collected = new HealingHerb();
            break;
        case SD3:
            item_collected = new TrapEnhancer();
            break;
        }

        if (item_collected->canUse(this))
        {
            item_collected->use(this);
            delete item_collected;
        }
        else
            this->getBag()->insert(item_collected);

        return true;
    }

    this->hp -= 100;
    return false;
}

// DragonLord
DragonLord::DragonLord(int index, const Position &init_pos, Map *map,
                       FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *groundteam)
    : MovingObject(index, init_pos, map, "DragonLord"),
      flyteam1(flyteam1), flyteam2(flyteam2), groundteam(groundteam), trap_step(-1), defeated(false), moved_step(0), hasSD(false) {}

int calcManhattanDistance(const Position &p1, const Position &p2)
{
    return abs(p1.getRow() - p2.getRow()) + abs(p1.getCol() - p2.getCol());
}

Position DragonLord::getNextPosition()
{
    Position pos1 = flyteam1->getCurrentPosition();
    Position pos2 = flyteam2->getCurrentPosition();

    int x = abs(pos1.getRow() - pos2.getRow());
    int y = abs(pos1.getCol() - pos2.getCol());

    Position next1(min(x, map->getNumRows() - 1), min(y, map->getNumCols() - 1));
    Position next2(min(y, map->getNumRows() - 1), min(x, map->getNumCols() - 1)); // đảo nếu cần

    int dist1 = calcManhattanDistance(next1, pos1);
    int dist2 = calcManhattanDistance(next1, pos2);

    // Nếu cả hai > 5 → giữ nguyên (x, y), ngược lại → đảo (y, x)
    Position final_pos = (dist1 > 5 && dist2 > 5) ? next1 : next2;

    if (map && map->isValid(final_pos, this))
        return final_pos;

    return Position::npos;
}

void DragonLord::move()
{

    trap_step -= 1;
    if (trap_step > 0)
        return;
    else if (trap_step == 0)
    {
        this->groundteam->reverse_trap();
        return;
    }

    cout << "MSG: " << this->name << " moved" << endl;

    Position next = getNextPosition();

    // Nếu có bước đi hợp lệ thì cập nhật vị trí
    if (!next.isEqual(Position::npos))
    {

        pos = next;
        moved_step += 1;
    }
}

string DragonLord::str() const
{
    return "DragonLord[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

void DragonLord::fight(MovingObject *warrior)
{
    // TODO
    if (!warrior->getCurrentPosition().isEqual(this->pos))
        return;

    if (warrior == this)
        return;

    if (auto sdra = dynamic_cast<SmartDragon *>(warrior))
    {
        cout << "MSG: " << this->getName() << " meets " << sdra->getName() << endl;
        return;
    }

    // Nếu là DragonLord
    if (auto lord = dynamic_cast<DragonLord *>(warrior))
    {
        cout << "MSG: " << this->getName() << " meets " << lord->getName() << endl;
        return;
    }

    if (auto fly = dynamic_cast<FlyTeam *>(warrior))
    {
        cout << "MSG: " << fly->getName() << " encounters " << this->getName() << endl;
        fly->attack(this);
        return;
    }

    if (auto ground = dynamic_cast<GroundTeam *>(warrior))
    {
        cout << "MSG: " << ground->getName() << " encounters " << this->getName() << endl;
        ground->trap(this);
        return;
    }
}

SmartDragon *DragonLord::creat_smart_dragon(Position prev, int &num_SD1, int &num_SD2, int &num_SD3)
{
    if (this->moved_step == 0)
        return nullptr;
    if (this->moved_step % 5 != 0)
        return nullptr;

    int r = prev.getRow();
    int c = prev.getCol();
    int damage = (r * 257 + c * 139 + 89) % 900 + 1;
    if (!hasSD)
    {
        hasSD = true;
        num_SD1 += 1;
        return new SmartDragon(5, prev, this->map, SD1, this->flyteam1, damage);
    }

    int d1 = calcManhattanDistance(prev, flyteam1->getCurrentPosition());
    int d2 = calcManhattanDistance(prev, flyteam2->getCurrentPosition());
    int d3 = calcManhattanDistance(prev, groundteam->getCurrentPosition());

    struct data
    {
        int dis;
        int num;
        DragonType idx;

        bool smaller(const data &a, const data &b)
        {
            if (a.dis != b.dis)
                return a.dis < b.dis;
            if (a.num != b.num)
                return a.num < b.num;
            return a.idx < b.idx; // giả sử DragonType so sánh được
        }
    };

    data sd1{d1, num_SD1, SD1};
    data sd2{d2, num_SD2, SD2};
    data sd3{d3, num_SD3, SD3};

    data arr[3] = {sd1, sd2, sd3};
    data choice = sd1;
    for (int i = 0; i < 3; i++)
    {
        if (choice.smaller(choice, arr[i]))
            continue;
        choice = arr[i];
    }

    MovingObject *target;
    if (choice.idx == SD1)
    {
        target = this->flyteam1;
        num_SD1 += 1;
    }
    else if (choice.idx == SD2)
    {
        target = this->flyteam2;
        num_SD2 += 1;
    }
    else if (choice.idx == SD3)
    {
        target = this->groundteam;
        num_SD3 += 1;
    }

    return new SmartDragon(5, prev, this->map, choice.idx, target, damage);
}

// ArrayMovingObject
ArrayMovingObject::ArrayMovingObject(int capacity)
    : capacity(capacity), count(0)
{
    arr_mv_objs = new MovingObject *[capacity];

    for (int i = 0; i < capacity; ++i)
        arr_mv_objs[i] = nullptr;
}

ArrayMovingObject::~ArrayMovingObject()
{
    for (int i = 0; i < count; ++i)
        arr_mv_objs[i] = nullptr;
    delete[] arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    return count >= capacity;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (isFull())
        return false;
    arr_mv_objs[count++] = mv_obj;
    return true;
}

string ArrayMovingObject::str() const
{
    string result = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";

    for (int i = 0; i < count; ++i)
    {
        result += arr_mv_objs[i]->str();
        if (i != count - 1)
            result += ";";
    }

    result += "]";
    return result;
}

int ArrayMovingObject::size() const
{
    return this->count;
}

MovingObject *ArrayMovingObject::get(int idx) const
{
    if (idx < 0 || idx >= count)
        return nullptr;
    return arr_mv_objs[idx];
}

bool ArrayMovingObject::remove(int idx)
{
    if (idx < 0 || idx >= count)
    {
        return false; // index không hợp lệ
    }

    // Giải phóng đối tượng nếu cần (tuỳ ownership)
    if (dynamic_cast<SmartDragon *>(arr_mv_objs[idx]))
        delete arr_mv_objs[idx]; // nếu ArrayMovingObject quản lý bộ nhớ

    // Dời các phần tử sau idx lên một bậc
    for (int i = idx; i < count - 1; i++)
    {
        arr_mv_objs[i] = arr_mv_objs[i + 1];
    }

    arr_mv_objs[count - 1] = nullptr; // optional: tránh dangling pointer
    count--;

    return true;
}

// SmartDragon
SmartDragon::SmartDragon(int index,
                         const Position &init_pos,
                         Map *map,
                         DragonType type,
                         MovingObject *target,
                         int damage)
    : MovingObject(index, init_pos, map, "SmartDragon"),
      smartdragon_type(type),
      damage(damage),
      item(nullptr),
      target(target),
      defeated(false)
{
    // tạo item theo type
    switch (smartdragon_type)
    {
    case SD1:
        item = new DragonScale();
        name = "SD1";
        break;
    case SD2:
        item = new HealingHerb();
        name = "SD2";
        break;
    case SD3:
        item = new TrapEnhancer();
        name = "SD3";
        break;
    }
}

SmartDragon::~SmartDragon()
{
    delete item;
    item = nullptr;
}

string SmartDragon::typeName() const
{
    switch (smartdragon_type)
    {
    case SD1:
        return "SD1";
    case SD2:
        return "SD2";
    case SD3:
        return "SD3";
    }
    return "SD?";
}

Position SmartDragon::getNextPosition()
{
    // SD3 đứng yên
    if (smartdragon_type == SD3)
        return Position::npos;

    if (!target)
        return Position::npos;

    Position cur = pos;
    Position tgt = target->getCurrentPosition();

    // Ưu tiên: UP, RIGHT, DOWN, LEFT (đúng đề)
    Position cand[4] = {
        Position(cur.getRow() - 1, cur.getCol()), // U
        Position(cur.getRow(), cur.getCol() + 1), // R
        Position(cur.getRow() + 1, cur.getCol()), // D
        Position(cur.getRow(), cur.getCol() - 1)  // L
    };

    int best = INT_MAX;
    Position bestPos = Position::npos;

    for (int i = 0; i < 4; ++i)
    {
        if (!map->isValid(cand[i], this))
            continue;
        int dist = abs(cand[i].getRow() - tgt.getRow()) +
                   abs(cand[i].getCol() - tgt.getCol());
        if (dist < best)
        {
            best = dist;
            bestPos = cand[i];
        }
    }
    return bestPos; // nếu không có bước hợp lệ -> npos
}

void SmartDragon::move()
{
    Position np = getNextPosition();
    if (!np.isEqual(-1, -1))
    {
        cout << "MSG: SmartDragon moved" << endl;
        pos = np;
    }
}

string SmartDragon::str() const
{
    // tg in ra bằng tên + pos nếu bạn có API phù hợp; tối thiểu in pos chính nó
    return "smartdragon[pos=" + pos.str() +
           ";type=" + string(typeName()) +
           ";tg=" + target->getName() + "]";
}

void SmartDragon::fight(MovingObject *warrior)
{
    if (!warrior->getCurrentPosition().isEqual(this->pos))
        return;

    if (warrior == this)
        return;

    // TODO
    if (!warrior->getCurrentPosition().isEqual(this->pos))
        return;

    if (auto sdra = dynamic_cast<SmartDragon *>(warrior))
    {
        cout << "MSG: " << this->getName() << " meets " << sdra->getName() << endl;
        return;
    }

    // Nếu là DragonLord
    if (auto lord = dynamic_cast<DragonLord *>(warrior))
    {
        cout << "MSG: " << this->getName() << " meets " << lord->getName() << endl;
        return;
    }

    if (auto war = dynamic_cast<Warrior *>(warrior))
    {
        cout << "MSG: " << this->getName() << " encounters " << war->getName() << endl;
        this->persued(war);
        return;
    }
}

bool SmartDragon::persued(Warrior *warrior)
{
    if (warrior->getDamage() >= this->damage)
    {
        // war thắng
        this->defeated = true;
        BaseItem *item_collected = this->item->clone();
        warrior->getBag()->insert(item_collected);
        return false;
    }
    warrior->setHp(warrior->getHp() - 100);
    return true;
}

// DragonScale
bool DragonScale::canUse(Warrior *w)
{
    if (w->getDamage() <= 400)
        return true;
    return false;
}

void DragonScale::use(Warrior *w)
{
    cout << "MSG: " << w->getName() << " used " << "DragonScale" << endl;
    int cur_damage = w->getDamage();
    int next_damage = round(cur_damage * 1.25);
    w->setDamage(next_damage);
}

// HealingHerb
bool HealingHerb::canUse(Warrior *w)
{
    if (w->getHp() <= 100)
        return true;
    return false;
}

void HealingHerb::use(Warrior *w)
{
    cout << "MSG: " << w->getName() << " used " << "HealingHerb" << endl;
    int cur_hp = w->getHp();
    int next_hp = round(cur_hp * 1.2);
    w->setHp(next_hp);
}

// TrapEnhancer
bool TrapEnhancer::canUse(Warrior *w)
{
    if (auto gt = dynamic_cast<GroundTeam *>(w))
        return true;
    return false;
}

void TrapEnhancer::use(Warrior *w)
{
    if (auto gt = dynamic_cast<GroundTeam *>(w))
    {
        cout << "MSG: " << w->getName() << " used " << "TrapEnhancer" << endl;
        gt->setTrapTurns(gt->getTrapTurns() + 1);
    }
}

// TeamBag
TeamBag::TeamBag(Warrior *w)
    : BaseBag(w), head(nullptr), count(0), capacity(0)
{
    if (dynamic_cast<FlyTeam *>(w))
    {
        capacity = 5;
    }
    else if (dynamic_cast<GroundTeam *>(w))
    {
        capacity = 7;
    }
    else
    {
        capacity = 0; // Phòng trường hợp không phải 2 loại trên
    }
}

TeamBag::~TeamBag()
{
    Node *curr = head;
    while (curr)
    {
        Node *tmp = curr;
        delete curr->item; // Giải phóng item
        curr = curr->next;
        delete tmp; // Giải phóng node
    }
    head = nullptr;
    count = 0;
}

bool TeamBag::insert(BaseItem *item)
{
    if (count >= capacity)
    {
        return false; // Túi đã đầy
    }

    Node *newNode = new Node(item, head); // chèn vào đầu
    head = newNode;
    count++;
    return true;
}

BaseItem *TeamBag::get()
{
    if (!head)
        return nullptr; // túi rỗng

    Node *prev = nullptr;
    Node *curr = head;

    // Tìm node có thể dùng
    while (curr && !curr->item->canUse(warrior))
    {
        prev = curr;
        curr = curr->next;
    }

    if (!curr)
        return nullptr; // không tìm thấy

    // Nếu curr không phải head → swap item giữa curr và head
    if (curr != head)
    {
        BaseItem *tempItem = head->item;
        head->item = curr->item;
        curr->item = tempItem;
    }

    // Bây giờ item cần lấy chắc chắn ở head
    Node *temp = head;
    head = head->next;
    BaseItem *foundItem = temp->item; // lấy item ra
    delete temp;                      // xóa node
    count--;

    return foundItem; // trả item cho caller
}

BaseItem *TeamBag::get(ItemType itemType)
{
    if (!head)
        return nullptr; // túi rỗng

    Node *prev = nullptr;
    Node *curr = head;

    // Tìm node có item type khớp
    while (curr && !isType(curr->item, itemType))
    {
        prev = curr;
        curr = curr->next;
    }

    if (!curr)
        return nullptr; // không tìm thấy

    // Nếu không phải node đầu → swap item với head
    if (curr != head)
    {
        BaseItem *tempItem = head->item;
        head->item = curr->item;
        curr->item = tempItem;
    }

    // Lấy item ở head và xóa node head
    Node *temp = head;
    head = head->next;
    BaseItem *foundItem = temp->item; // caller sẽ quản lý
    delete temp;                      // chỉ xóa node, không xóa item
    count--;

    return foundItem;
}

string TeamBag::str() const
{
    ostringstream oss;
    oss << "Bag[count=" << count << ";";

    Node *curr = head;
    while (curr)
    {
        if (dynamic_cast<DragonScale *>(curr->item))
        {
            oss << "DragonScale";
        }
        else if (dynamic_cast<HealingHerb *>(curr->item))
        {
            oss << "HealingHerb";
        }
        else if (dynamic_cast<TrapEnhancer *>(curr->item))
        {
            oss << "TrapEnhancer";
        }
        else
        {
            oss << "Unknown";
        }

        if (curr->next)
            oss << ",";
        curr = curr->next;
    }

    oss << "]";
    return oss.str();
}

// Configuration
Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Cannot open file: " << filepath << endl;
        return;
    }

    int lineCount = 0;
    string line;

    struct Attr
    {
        string name;
        string content;
        int idx;
    };

    Attr arr[21] = {
        {"MAP_NUM_ROWS", "0", 0},
        {"MAP_NUM_COLS", "0", 1},
        {"MAX_NUM_MOVING_OBJECTS", "0", 2},
        {"NUM_OBSTACLE", "0", 3},
        {"ARRAY_OBSTACLE", "[]", 4},
        {"NUM_GROUND_OBSTACLE", "0", 5},
        {"ARRAY_GROUND_OBSTACLE", "[]", 6},
        {"FLYTEAM1_MOVING_RULE", "", 7},
        {"FLYTEAM1_INIT_POS", "(0,0)", 8},
        {"FLYTEAM1_INIT_HP", "0", 9},
        {"FLYTEAM1_INIT_DAMAGE", "0", 10},
        {"FLYTEAM2_MOVING_RULE", "", 11},
        {"FLYTEAM2_INIT_POS", "(0,0)", 12},
        {"FLYTEAM2_INIT_HP", "0", 13},
        {"FLYTEAM2_INIT_DAMAGE", "0", 14},
        {"GROUNDTEAM_MOVING_RULE", "", 15},
        {"GROUNDTEAM_INIT_POS", "(0,0)", 16},
        {"GROUNDTEAM_INIT_HP", "0", 17},
        {"GROUNDTEAM_INIT_DAMAGE", "0", 18},
        {"DRAGONLORD_INIT_POS", "(0,0)", 19},
        {"NUM_STEPS", "0", 20}};

    while (getline(file, line))
    {
        // cout << line << endl;
        for (int i = 0; i < 21; i++)
        {
            // Nếu tìm thấy tên attr trong dòng
            if (line.find(arr[i].name) != string::npos)
            {
                size_t pos = line.find('=');
                if (pos != string::npos)
                {
                    // Lấy phần sau dấu '='
                    arr[i].content = removeSpaces(line.substr(pos + 1));
                }
                break; // đã tìm thấy thì thoát vòng for
            }
        }
    }

    file.close();

    // Gán vào thuộc tính
    this->map_num_rows = stoi(arr[0].content);
    this->map_num_cols = stoi(arr[1].content);
    this->max_num_moving_objects = stoi(arr[2].content);

    this->num_obstacles = this->countChar(arr[4].content, '(');
    this->arr_obstacles = handle_arr_pos(arr[4].content, this->num_obstacles);

    this->num_ground_obstacles = this->countChar(arr[6].content, '(');
    this->arr_ground_obstacles = handle_arr_pos(arr[6].content, this->num_ground_obstacles);

    this->flyteam1_moving_rule = arr[7].content;
    this->flyteam1_init_pos = Position(arr[8].content);
    this->flyteam1_init_hp = stoi(arr[9].content);
    this->flyteam1_init_damage = stoi(arr[10].content);

    this->flyteam2_moving_rule = arr[11].content;
    this->flyteam2_init_pos = Position(arr[12].content);
    this->flyteam2_init_hp = stoi(arr[13].content);
    this->flyteam2_init_damage = stoi(arr[14].content);

    this->groundteam_moving_rule = arr[15].content;
    this->groundteam_init_pos = Position(arr[16].content);
    this->groundteam_init_hp = stoi(arr[17].content);
    this->groundteam_init_damage = stoi(arr[18].content);

    this->dragonlord_init_pos = Position(arr[19].content);
    this->num_steps = stoi(arr[20].content);
}

Configuration::~Configuration()
{
    this->freeArrays();
}

string Configuration::str() const
{
    ostringstream oss;
    oss << "Configuration[\n";
    oss << "MAP_NUM_ROWS=" << map_num_rows << "\n";
    oss << "MAP_NUM_COLS=" << map_num_cols << "\n";
    oss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << "\n";
    oss << "NUM_OBSTACLE=" << num_obstacles << "\n";

    oss << "ARRAY_OBSTACLE=[";
    for (int i = 0; i < num_obstacles; i++)
    {
        oss << arr_obstacles[i].str();
        if (i != num_obstacles - 1)
            oss << ";";
    }
    oss << "]\n";

    oss << "NUM_GROUND_OBSTACLE=" << num_ground_obstacles << "\n";

    oss << "ARRAY_GROUND_OBSTACLE=[";
    for (int i = 0; i < num_ground_obstacles; i++)
    {
        oss << arr_ground_obstacles[i].str();
        if (i != num_ground_obstacles - 1)
            oss << ";";
    }
    oss << "]\n";

    oss << "FLYTEAM1_MOVING_RULE=" << flyteam1_moving_rule << "\n";
    oss << "FLYTEAM1_INIT_POS=" << flyteam1_init_pos.str() << "\n";
    oss << "FLYTEAM1_INIT_HP=" << flyteam1_init_hp << "\n";
    oss << "FLYTEAM1_INIT_DAMAGE=" << flyteam1_init_damage << "\n";

    oss << "FLYTEAM2_MOVING_RULE=" << flyteam2_moving_rule << "\n";
    oss << "FLYTEAM2_INIT_POS=" << flyteam2_init_pos.str() << "\n";
    oss << "FLYTEAM2_INIT_HP=" << flyteam2_init_hp << "\n";
    oss << "FLYTEAM2_INIT_DAMAGE=" << flyteam2_init_damage << "\n";

    oss << "GROUNDTEAM_MOVING_RULE=" << groundteam_moving_rule << "\n";
    oss << "GROUNDTEAM_INIT_POS=" << groundteam_init_pos.str() << "\n";
    oss << "GROUNDTEAM_INIT_HP=" << groundteam_init_hp << "\n";
    oss << "GROUNDTEAM_INIT_DAMAGE=" << groundteam_init_damage << "\n";

    oss << "DRAGONLORD_INIT_POS=" << dragonlord_init_pos.str() << "\n";
    oss << "NUM_STEPS=" << num_steps << "\n";
    oss << "]";
    return oss.str();
}

// DragonWarriorsProgram
DragonWarriorsProgram::DragonWarriorsProgram(const string &config_file_path)
{
    // 1. Đọc cấu hình
    this->config = new Configuration(config_file_path);

    // 2. Tạo bản đồ từ dữ liệu config (dùng getter)
    int map_rows = config->getMapNumRows();
    int map_cols = config->getMapNumCols();

    int num_obst = config->getNumObstacles();
    Position *obst_arr = config->getArrObstacles();
    int num_ground_obst = config->getNumGroundObstacles();
    Position *ground_obst_arr = config->getArrGroundObstacles();

    Map *game_map = new Map(map_rows, map_cols,
                            num_obst, obst_arr,
                            num_ground_obst, ground_obst_arr);
    this->map = game_map;

    // 3. Tạo các đội FlyTeam và GroundTeam (dùng getter)
    string f1_rule = config->getFlyTeam1MovingRule();
    Position f1_pos = config->getFlyTeam1InitPos();
    int f1_hp = config->getFlyTeam1InitHP();
    int f1_dmg = config->getFlyTeam1InitDamage();

    FlyTeam *team_f1 = new FlyTeam(1, f1_rule, f1_pos, game_map, f1_hp, f1_dmg);
    this->flyteam1 = team_f1;

    string f2_rule = config->getFlyTeam2MovingRule();
    Position f2_pos = config->getFlyTeam2InitPos();
    int f2_hp = config->getFlyTeam2InitHP();
    int f2_dmg = config->getFlyTeam2InitDamage();

    FlyTeam *team_f2 = new FlyTeam(2, f2_rule, f2_pos, game_map, f2_hp, f2_dmg);
    this->flyteam2 = team_f2;

    string g_rule = config->getGroundTeamMovingRule();
    Position g_pos = config->getGroundTeamInitPos();
    int g_hp = config->getGroundTeamInitHP();
    int g_dmg = config->getGroundTeamInitDamage();
    int g_trap_turns = 0; // chưa có thông tin trap_turns trong file config

    GroundTeam *team_g = new GroundTeam(3, g_rule, g_pos, game_map, g_hp, g_dmg);
    this->groundteam = team_g;

    // 4. Tạo DragonLord
    Position dl_pos = config->getDragonLordInitPos();
    DragonLord *lord = new DragonLord(0, dl_pos, game_map, team_f1, team_f2, team_g);
    this->dragonlord = lord;

    // 5. Khởi tạo ArrayMovingObject và thêm các đối tượng
    int max_objs = config->getMaxNumMovingObjects();
    ArrayMovingObject *arr_objs = new ArrayMovingObject(max_objs);
    arr_objs->add(team_f1);
    arr_objs->add(team_f2);
    arr_objs->add(team_g);
    arr_objs->add(lord);
    this->arr_mv_objs = arr_objs;
}

DragonWarriorsProgram::~DragonWarriorsProgram()
{
    delete config;
    delete flyteam1;
    delete flyteam2;
    delete groundteam;
    delete dragonlord;
    delete map;
    delete arr_mv_objs;
}

bool DragonWarriorsProgram::isStop() const
{
    int fly1_hp = this->flyteam1->getHp();
    int fly2_hp = this->flyteam2->getHp();
    int gro_hp = this->groundteam->getHp();
    if (fly1_hp <= 1 && fly2_hp <= 1)
        return true;
    if (fly1_hp <= 1 && fly2_hp <= 1 && gro_hp <= 1)
        return true;
    // Position lord_pos = this->dragonlord->getCurrentPosition();
    // Position fly1_pos = this->flyteam1->getCurrentPosition();
    // Position fly2_pos = this->flyteam2->getCurrentPosition();
    if (dragonlord->defeated)
        return true;

    return false;
}

void DragonWarriorsProgram::run(bool verbose)
{
    int num_SD1 = 0;
    int num_SD2 = 0;
    int num_SD3 = 0;
    for (int step = 0; step < this->config->getNumSteps(); step++)
    {

        Position prev = this->dragonlord->getCurrentPosition();
        SmartDragon *newsd = nullptr;
        for (int idx_obj = 0; idx_obj < this->arr_mv_objs->size(); idx_obj++)
        {

            MovingObject *obj = this->arr_mv_objs->get(idx_obj);
            Position prev = obj->getCurrentPosition();
            obj->move();

            if (auto war = dynamic_cast<Warrior *>(obj))
            {
                if (war->getHp() <= 1)
                {
                    this->arr_mv_objs->remove(idx_obj--);
                    if (this->isStop())
                    {
                        if (verbose)
                            this->printStep(step);
                        this->printResult();
                        return;
                    }
                    continue;
                }
            }

            if (auto lord = dynamic_cast<DragonLord *>(obj))
            {
                if (lord->getCurrentPosition().isEqual(prev))
                    continue;
            }

            for (int enemy = 0; enemy < this->arr_mv_objs->size(); enemy++)
            {
                MovingObject *enemy_obj = this->arr_mv_objs->get(enemy);
                obj->fight(enemy_obj);
                if (this->isStop())
                {
                    if (verbose)
                        this->printStep(step);
                    this->printResult();
                    return;
                }
            }

            if (this->dragonlord->moved_step % 5 == 0 && dynamic_cast<DragonLord *>(obj))
            {
                if (this->arr_mv_objs->isFull())
                {
                    cout << "MSG: ArrayMovingObjects is full. Cannot create more SmartDragon" << endl;
                }
                else
                {
                    newsd = dragonlord->creat_smart_dragon(prev, num_SD1, num_SD2, num_SD3);
                    cout << "MSG: " << newsd->getName() << " created at " << prev.str() << endl;
                }
            }

            for (int dead = 0; dead < this->arr_mv_objs->size(); dead++)
            {
                MovingObject *dead_obj = this->arr_mv_objs->get(dead);
                if (auto war = dynamic_cast<Warrior *>(dead_obj))
                {
                    if (war->getHp() <= 1)
                    {
                        this->arr_mv_objs->remove(dead);
                        if (dead <= idx_obj)
                        {
                            idx_obj--;
                        }
                        dead--;
                    }
                }
                else if (auto sd = dynamic_cast<SmartDragon *>(dead_obj))
                {
                    if (sd->defeated)
                    {
                        this->arr_mv_objs->remove(dead);
                        if (dead <= idx_obj)
                        {
                            idx_obj--;
                        }
                        dead--;
                    }
                }
            }
        }

        if (newsd != nullptr)
        {
            this->arr_mv_objs->add(newsd);
            newsd = nullptr;
        }
        if (verbose)
            this->printStep(step);
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;

        // if (this->isStop())
        // {
        //     this->printResult();
        //     break;
        // }
    }
    this->printResult();
}
