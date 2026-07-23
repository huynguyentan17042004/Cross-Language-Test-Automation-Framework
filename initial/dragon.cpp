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

    grid = new MapElement **[num_rows];
    for (int r = 0; r < num_rows; ++r)
        grid[r] = new MapElement *[num_cols];

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
                grid[r][c] = new MapElement(GROUND_OBSTACLE, req_dmg);
            }
            else
                grid[r][c] = new MapElement(type); // OBSTACLE hoặc PATH
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

    MapElement *element = grid[r][c];
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
    bag = nullptr;
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
    if (hp < 0)
        this->hp = 0;
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
}

FlyTeam::~FlyTeam()
{
    --count;
    delete bag;
}

Position FlyTeam::getNextPosition()
{
    // Lấy vị trí hiện tại
    int r = pos.getRow();
    int c = pos.getCol();

    // Ký tự hướng di chuyển hiện tại
    if (moving_index >= moving_rule.length())
        return Position::npos;
    char dir = moving_rule[moving_index];

    // Tính toán vị trí kế tiếp
    int new_r = r;
    int new_c = c;

    switch (dir)
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
        return Position::npos; // Nếu ký tự không hợp lệ
    }

    Position next_pos(new_r, new_c);

    // Nếu vị trí mới hợp lệ → tăng chỉ số và trả về
    if (map && map->isValid(next_pos, this))
        return next_pos;

    // Không hợp lệ → không tăng index, trả về npos
    return Position::npos;
}

void FlyTeam::move()
{
    Position next = getNextPosition();

    // Nếu có bước đi hợp lệ thì cập nhật vị trí
    if (!next.isEqual(Position::npos.getRow(), Position::npos.getCol()))
    {
        pos = next;
        moving_index++;
    }
}

string FlyTeam::str() const
{
    return this->name +
           "[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";moving_rule=" + moving_rule + "]";
}

// GroundTeam
GroundTeam::GroundTeam(int index, const string &moving_rule, const Position &pos,
                       Map *map, int init_hp, int init_damage)
    : Warrior(index, pos, map, "GroundTeam", init_hp, init_damage)
{
    this->moving_rule = moving_rule;
    this->moving_index = 0; // bắt đầu từ ký tự đầu tiên trong moving_rule ? moving index
    this->trap_turns = 0;
    this->bag = new TeamBag(this);
}

GroundTeam::~GroundTeam()
{
    delete bag;
}

Position GroundTeam::getNextPosition()
{
    // Lấy vị trí hiện tại
    int r = pos.getRow();
    int c = pos.getCol();

    // Ký tự hướng di chuyển hiện tại
    if (moving_index >= moving_rule.length())
        return Position::npos;
    char dir = moving_rule[moving_index];

    // Tính toán vị trí kế tiếp
    int new_r = r;
    int new_c = c;

    switch (dir)
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
        return Position::npos; // Nếu ký tự không hợp lệ
    }

    Position next_pos(new_r, new_c);

    // Nếu vị trí mới hợp lệ → tăng chỉ số và trả về
    if (map && map->isValid(next_pos, this))
        return next_pos;

    // Không hợp lệ → không tăng index, trả về npos
    return Position::npos;
}

void GroundTeam::move()
{
    Position next = getNextPosition();

    // Nếu có bước đi hợp lệ thì cập nhật vị trí
    if (!next.isEqual(Position::npos.getRow(), Position::npos.getCol()))
    {
        pos = next;
        moving_index++;
    }
}

string GroundTeam::str() const
{
    return "GroundTeam[index=" + to_string(index) +
           ";pos=" + pos.str() +
           ";moving_rule=" + moving_rule + "]";
}

bool GroundTeam::trap(DragonLord *dragon)
{
    int r = dragon->getCurrentPosition().getRow();
    int c = dragon->getCurrentPosition().getCol();
    if (this->getCurrentPosition().isEqual(r, c))
    {
        if (this->trap_turns > 0)
        {
            dragon->trap_step = this->trap_turns;
            return true;
        }
        // else
        // {
        //     this->pos = Position(c, r);
        //     this->hp -= 200;
        // }
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

// DragonLord
DragonLord::DragonLord(int index, const Position &init_pos, Map *map,
                       FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *groundteam)
    : MovingObject(index, init_pos, map, "DragonLord"),
      flyteam1(flyteam1), flyteam2(flyteam2), groundteam(groundteam), trap_step(0) {}

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

    Position next1(x, y);
    Position next2(y, x); // đảo nếu cần

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
    if (this->trap_step != 0)
    {
        this->trap_step -= 1;
        return;
    }
    Position next = getNextPosition();

    // Nếu có bước đi hợp lệ thì cập nhật vị trí
    if (!next.isEqual(Position::npos.getRow(), Position::npos.getCol()))
        pos = next;
}

string DragonLord::str() const
{
    return "DragonLord[index=" + to_string(index) + ";pos=" + pos.str() + "]";
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
      target(target)
{
    // tạo item theo type
    switch (smartdragon_type)
    {
    case SD1:
        item = new DragonScale();
        break;
    case SD2:
        item = new HealingHerb();
        break;
    case SD3:
        item = new TrapEnhancer();
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
        pos = np;
}

string SmartDragon::str() const
{
    // tg in ra bằng tên + pos nếu bạn có API phù hợp; tối thiểu in pos chính nó
    return "smartdragon[pos=" + pos.str() +
           ";type=" + string(typeName()) +
           ";tg=" + target->str() + "]";
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
    int cur_damage = w->getDamage();
    int next_damage = cur_damage * 1.25;
    w->setDamage(next_damage);
}

// HealingHerb
bool HealingHerb::canUse(Warrior *w)
{
    if (w->getHp() <= 10)
        return true;
    return false;
}

void HealingHerb::use(Warrior *w)
{
    int cur_hp = w->getHp();
    int next_hp = cur_hp * 1.2;
    w->setHp(next_hp);
}

// TrapEnhancer
bool TrapEnhancer::canUse(Warrior *w)
{
    return true;
}

void TrapEnhancer::use(Warrior *w)
{
    GroundTeam *gt = dynamic_cast<GroundTeam *>(w);
    if (gt)
    {
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
        {"FLYTEAM1_INIT_D", "0", 10},
        {"FLYTEAM2_MOVING_RULE", "", 11},
        {"FLYTEAM2_INIT_POS", "(0,0)", 12},
        {"FLYTEAM2_INIT_HP", "0", 13},
        {"FLYTEAM2_INIT_D", "0", 14},
        {"GROUNDTEAM_MOVING_RULE", "", 15},
        {"GROUNDTEAM_INIT_POS", "(0,0)", 16},
        {"GROUNDTEAM_INIT_HP", "0", 17},
        {"GROUNDTEAM_INIT_D", "0", 18},
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

    this->num_obstacles = stoi(arr[3].content);
    this->countChar(arr[4].content, '(');
    this->arr_obstacles = handle_arr_pos(arr[4].content, this->countChar(arr[4].content, '('));

    this->num_ground_obstacles = stoi(arr[5].content);
    this->countChar(arr[6].content, '(');
    this->arr_ground_obstacles = handle_arr_pos(arr[6].content, this->countChar(arr[6].content, '('));

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
    delete[] arr_obstacles;
    arr_obstacles = nullptr;

    delete[] arr_ground_obstacles;
    arr_ground_obstacles = nullptr;
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
    oss << "FLYTEAM1_INIT_DMG=" << flyteam1_init_damage << "\n";

    oss << "FLYTEAM2_MOVING_RULE=" << flyteam2_moving_rule << "\n";
    oss << "FLYTEAM2_INIT_POS=" << flyteam2_init_pos.str() << "\n";
    oss << "FLYTEAM2_INIT_HP=" << flyteam2_init_hp << "\n";
    oss << "FLYTEAM2_INIT_DMG=" << flyteam2_init_damage << "\n";

    oss << "GROUNDTEAM_MOVING_RULE=" << groundteam_moving_rule << "\n";
    oss << "GROUNDTEAM_INIT_POS=" << groundteam_init_pos.str() << "\n";
    oss << "GROUNDTEAM_INIT_HP=" << groundteam_init_hp << "\n";
    oss << "GROUNDTEAM_INIT_DMG=" << groundteam_init_damage << "\n";

    oss << "DRAGONLORD_INIT_POS=" << dragonlord_init_pos.str() << "\n";
    oss << "NUM_STEPS=" << num_steps << "\n";
    oss << "]";
    return oss.str();
}

// DragonWarriorsProgram
DragonWarriorsProgram::DragonWarriorsProgram(const string &config_file_path)
{
    // 1. Đọc cấu hình
    Configuration *cfg = new Configuration(config_file_path);
    this->config = cfg;

    // 2. Tạo bản đồ từ dữ liệu config
    int map_rows = cfg->map_num_rows;
    int map_cols = cfg->map_num_cols;
    int num_obst = cfg->num_obstacles;
    Position *obst_arr = cfg->arr_obstacles;
    int num_ground_obst = cfg->num_ground_obstacles;
    Position *ground_obst_arr = cfg->arr_ground_obstacles;

    Map *game_map = new Map(map_rows, map_cols,
                            num_obst, obst_arr,
                            num_ground_obst, ground_obst_arr);
    this->map = game_map;

    // 3. Tạo các đội FlyTeam và GroundTeam
    string f1_rule = cfg->flyteam1_moving_rule;
    Position f1_pos = cfg->flyteam1_init_pos;
    int f1_hp = cfg->flyteam1_init_hp;
    int f1_dmg = cfg->flyteam1_init_damage;

    FlyTeam *team_f1 = new FlyTeam(1, f1_rule, f1_pos, game_map, f1_hp, f1_dmg);
    this->flyteam1 = team_f1;

    string f2_rule = cfg->flyteam2_moving_rule;
    Position f2_pos = cfg->flyteam2_init_pos;
    int f2_hp = cfg->flyteam2_init_hp;
    int f2_dmg = cfg->flyteam2_init_damage;

    FlyTeam *team_f2 = new FlyTeam(2, f2_rule, f2_pos, game_map, f2_hp, f2_dmg);
    this->flyteam2 = team_f2;

    string g_rule = cfg->groundteam_moving_rule;
    Position g_pos = cfg->groundteam_init_pos;
    int g_hp = cfg->groundteam_init_hp;
    int g_dmg = cfg->groundteam_init_damage;
    int g_trap_turns = 0; // Chưa có thông tin trap_turns trong file config

    GroundTeam *team_g = new GroundTeam(3, g_rule, g_pos, game_map, g_hp, g_dmg);
    this->groundteam = team_g;

    // 4. Tạo DragonLord
    Position dl_pos = cfg->dragonlord_init_pos;
    DragonLord *lord = new DragonLord(0, dl_pos, game_map, team_f1, team_f2, team_g);
    this->dragonlord = lord;

    // 5. Khởi tạo ArrayMovingObject và thêm các đối tượng
    int max_objs = cfg->max_num_moving_objects;
    ArrayMovingObject *arr_objs = new ArrayMovingObject(max_objs);
    arr_objs->add(lord);
    arr_objs->add(team_f1);
    arr_objs->add(team_f2);
    arr_objs->add(team_g);
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

    Position lord_pos = this->dragonlord->getCurrentPosition();
    Position fly1_pos = this->flyteam1->getCurrentPosition();
    Position fly2_pos = this->flyteam2->getCurrentPosition();
    if (fly1_pos.isEqual(lord_pos.getRow(), lord_pos.getCol()) ||
        fly2_pos.isEqual(lord_pos.getRow(), lord_pos.getCol()))
        return true;

    return false;
}

void DragonWarriorsProgram::run(bool verbose)
{
    int lord_step = 0;
    int sd1 = 0, sd2 = 0, sd3 = 0;

    // Lưu bước spawn đầu tiên của từng loại SD, -1 = chưa xuất hiện
    int sd1_1st_occur = -1;
    int sd2_1st_occur = -1;
    int sd3_1st_occur = -1;

    int back = -1;

    for (int step = 0; step < this->config->num_steps; step++)
    {
        for (int idx_obj = 0; idx_obj < this->arr_mv_objs->count; idx_obj++)
        {
            MovingObject *obj = this->arr_mv_objs->arr_mv_objs[idx_obj];

            // còn thiếu vụ turn nha
            if (dynamic_cast<DragonLord *>(obj))
            {
                // DragonLord *lord = dynamic_cast<DragonLord *>(obj);
                if (obj->getNextPosition().isEqual(-1, -1))
                    continue;

                lord_step++;

                if (lord_step == 5)
                {
                    lord_step = 0;

                    Position lord_pos = this->dragonlord->getCurrentPosition();
                    int r = lord_pos.getRow();
                    int c = lord_pos.getCol();
                    int req_dmg = (r * 257 + c * 139 + 89) % 900 + 1;

                    MovingObject *target = nullptr;
                    DragonType chosenType;

                    if (sd1 + sd2 + sd3 == 0)
                    {
                        chosenType = SD1;
                        target = this->flyteam1;
                        sd1++;
                        sd1_1st_occur = step;
                    }
                    else
                    {
                        int dist1 = calcManhattanDistance(lord_pos, this->flyteam1->getCurrentPosition());
                        int dist2 = calcManhattanDistance(lord_pos, this->flyteam2->getCurrentPosition());
                        int dist3 = calcManhattanDistance(lord_pos, this->groundteam->getCurrentPosition());

                        int minDist = min(dist1, min(dist2, dist3));

                        struct Candidate
                        {
                            DragonType type;
                            int count;
                            int firstOccur;
                            MovingObject *tgt;
                        };
                        Candidate candidates[3];
                        int candCount = 0;

                        if (dist1 == minDist)
                            candidates[candCount++] = {SD1, sd1, sd1_1st_occur, this->flyteam1};
                        if (dist2 == minDist)
                            candidates[candCount++] = {SD2, sd2, sd2_1st_occur, this->flyteam2};
                        if (dist3 == minDist)
                            candidates[candCount++] = {SD3, sd3, sd3_1st_occur, this->groundteam};

                        // Tìm min count + first occur
                        int bestIdx = 0;
                        int minFirstOccur = candidates[0].firstOccur;
                        int minCount = candidates[0].count;
                        for (int i = 1; i < candCount; i++)
                            if (candidates[i].count < minCount)
                            {
                                minCount = candidates[i].count;
                                bestIdx = i;
                            }
                            else if (candidates[i].count == minCount)
                            {
                                if (candidates[i].firstOccur < minFirstOccur)
                                {
                                    minFirstOccur = candidates[i].firstOccur;
                                    bestIdx = i;
                                }
                            }

                        // Lọc theo số lượng ít nhất
                        chosenType = candidates[bestIdx].type;
                        target = candidates[bestIdx].tgt;

                        // Tăng số lượng & lưu lần xuất hiện đầu tiên nếu chưa có
                        if (chosenType == SD1)
                        {
                            sd1++;
                            if (sd1_1st_occur == -1)
                                sd1_1st_occur = step;
                        }
                        else if (chosenType == SD2)
                        {
                            sd2++;
                            if (sd2_1st_occur == -1)
                                sd2_1st_occur = step;
                        }
                        else
                        {
                            sd3++;
                            if (sd3_1st_occur == -1)
                                sd3_1st_occur = step;
                        }
                    }

                    if (!this->arr_mv_objs->isFull())
                    {
                        SmartDragon *sd = new SmartDragon(this->arr_mv_objs->count, lord_pos, this->map, chosenType, target, req_dmg);
                        this->arr_mv_objs->add(sd);
                    }
                }
            }

            // if warrior là ground team check back và trừ 400 máu
            obj->move();
            // chiển bình gặp rồng
            if (dynamic_cast<Warrior *>(obj))
            {
                Warrior *war_obj = dynamic_cast<Warrior *>(obj);
                for (int idx = 0; idx < this->arr_mv_objs->count; idx++)
                {
                    MovingObject *objam = this->arr_mv_objs->arr_mv_objs[idx];
                    if (!obj->getCurrentPosition().isEqual(objam->getCurrentPosition()))
                        continue;

                    if (dynamic_cast<DragonLord *>(objam))
                    {
                        // TODO
                        if (dynamic_cast<FlyTeam *>(war_obj))
                            break;

                        GroundTeam *gro_team = dynamic_cast<GroundTeam *>(war_obj);
                        gro_team->trap(this->dragonlord);
                        back = gro_team->getTrapTurns();
                    }
                    else if ((dynamic_cast<SmartDragon *>(objam)))
                    {
                        // TODO
                        //  Mỗi loại rồng thì cần chiến thắng // lưu ý nhé
                        SmartDragon *sm_dra = dynamic_cast<SmartDragon *>(objam);
                        if (war_obj->getDamage() <= sm_dra->getDmg())
                        {
                            int hp = war_obj->getHp();
                            hp -= 100;
                            war_obj->setHp(hp);
                        }
                        else
                        {
                            DragonType typ = sm_dra->getDragonType();
                            BaseItem *item;
                            if (typ == SD1)
                            {
                                item = new DragonScale();
                            }
                            else if (typ == SD2)
                            {
                                item = new HealingHerb();
                            }
                            else if (typ == SD3)
                            {
                                if (dynamic_cast<GroundTeam *>(war_obj))
                                    item = new TrapEnhancer();
                                else
                                {
                                    bool isFly1 = (war_obj->str().find("FlyTeam1") != string::npos);
                                    if (isFly1)
                                        item = new DragonScale();
                                    else
                                        item = new DragonScale();
                                }
                            }

                            // check if can use ->use hoac insert
                            if (item->canUse(war_obj))
                            {
                                item->use(war_obj);
                                delete item;
                            }
                            else
                            {
                                war_obj->getBag()->insert(item);
                            }
                        }
                    }
                }
            }
            else if (dynamic_cast<DragonLord *>(obj))
            {
                DragonLord *lord = dynamic_cast<DragonLord *>(obj);
                for (int idx = 0; idx < this->arr_mv_objs->count; idx++)
                {
                    MovingObject *objam = this->arr_mv_objs->arr_mv_objs[idx];
                    if (!obj->getCurrentPosition().isEqual(objam->getCurrentPosition()))
                        continue;
                    if ((dynamic_cast<SmartDragon *>(objam)))
                        continue;
                    if (dynamic_cast<FlyTeam *>(objam))
                        break;

                    // nếu gặp team đất thì 5 lượt tiếp theo ấy
                }
            }
            else if ((dynamic_cast<SmartDragon *>(obj)))
            {
                SmartDragon *sm_dra = dynamic_cast<SmartDragon *>(obj);
                for (int idx = 0; idx < this->arr_mv_objs->count; idx++)
                {
                    MovingObject *objam = this->arr_mv_objs->arr_mv_objs[idx];
                    if (!obj->getCurrentPosition().isEqual(objam->getCurrentPosition()))
                        continue;

                    if (dynamic_cast<DragonLord *>(objam))
                        continue;

                    Warrior *war_obj = dynamic_cast<Warrior *>(objam);
                    DragonType typ = sm_dra->getDragonType();
                    BaseItem *item;
                    if (typ == SD1)
                    {
                        item = new DragonScale();
                    }
                    else if (typ == SD2)
                    {
                        item = new HealingHerb();
                    }
                    else if (typ == SD3)
                    {
                        if (dynamic_cast<GroundTeam *>(war_obj))
                            item = new TrapEnhancer();
                        else
                        {
                            bool isFly1 = (war_obj->str().find("FlyTeam1") != string::npos);
                            if (isFly1)
                                item = new DragonScale();
                            else
                                item = new HealingHerb();
                        }
                    }
                    war_obj->getBag()->insert(item);
                }
            }
            // rồng gặp chiến binh
        }

        if (verbose)
            cout << this->arr_mv_objs->str() << endl;

        if (this->isStop())
            break;

        back--;
    }
}
