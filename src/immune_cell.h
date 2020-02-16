#ifndef IMMUNE_CELL_H
#define IMMUNE_CELL_H

class World;

enum class ImmuneState {
    VIRGIN,
    MATURE,
    DEAD
};

class ImmuneCell {
public:
    int x, y;
    int age;
    ImmuneState state;

    ImmuneCell(int x, int y, int age, ImmuneState state);

    void Update(World& world);

private:
    // Move the immune cell
    void Move();

    // Updates
    void UpdateVirgin(World& world);
    void UpdateMature(World& world);

    // Dead check
    int IsDeadFromOldAge();
};

#endif