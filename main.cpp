#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates.h"


using namespace std;

enum class FurnitureColor : int
{
    Brown,
    Beige,
    Transparent,
    Unknown
};

class Furniture {
private:
    string material;
    FurnitureColor color;
    int weight;

protected:
    bool FurnitureIsExpen; // "дорогая мебель?"

public:
    Furniture(string mater, FurnitureColor clr, int wgt)
        : material(mater), color(clr), weight(wgt)
        {
            FurnitureIsExpen = static_cast<bool>(rand()%2);
        }

    virtual void assemble() {
        cout << "Furniture assembled" << endl;
    }
    virtual void disassemble() {
        cout << "Furniture disassembled" << endl;
    }

    virtual void clean() = 0;

    virtual void punch() {
        cout << "PUNCH furniture" << endl;
    }
    FurnitureColor GetColor() const { return color; }
    virtual ~Furniture() {}
    bool IsExpen() const { return FurnitureIsExpen; }
};

class Chair : public Furniture {
private:
    int numLegs; // ножки
    bool hasBack; // имеется спинка

public:
    void assemble() override {
    cout << "Chair assembly with " << numLegs << " legs."
          << (hasBack ? " Has back" : "") << endl;
    }
    void clean() override;
    Chair();
    ~Chair() {}
};

Chair::Chair() : Furniture("Wood", FurnitureColor::Brown, 5), numLegs(4), hasBack(true)
{
}

void Chair::clean()
{
    cout << "Clean a chair..." << endl;
}

class Table : public Furniture {
private:
    string shape;
    int numSeats;

public:
    void assemble() override {
        cout << "Table assembly with " << shape << " shape and with "
                  << numSeats << " num of seats." << endl;
    }

    void clean();
    Table();
    ~Table() {}
};

Table::Table() : Furniture("Table", FurnitureColor::Transparent, 11), shape("round"), numSeats(5)
{
}

void Table::clean()
{
    cout << "Wipe a table..." << endl;
}

class Sofa : public Furniture {
private:
    int numSeats;
    bool canConvertToBed; //может ли разложиться в спальное место(кровать)

public:
    void assemble() override {
        cout << "Sofa assembly with  " << numSeats << " seats."
                  << (canConvertToBed ? " Can convert to bed?. " : "") << endl;
    }

    ~Sofa() {}
    Sofa();
    void clean() override;
};
Sofa::Sofa() : Furniture("Fabric", FurnitureColor::Beige, 25), numSeats(4), canConvertToBed(true)
{
}


void Sofa::clean()
{
    cout << "Vacuum a sofa..." << endl;
}

enum class FurnitureType : int
{
    Chair = 1,
    Table = 2,
    Sofa = 3,

    Undefined = 0
};

Furniture *CreateFurniture(FurnitureType type)
{
    Furniture *newFurniture = nullptr;
    if (type == FurnitureType::Chair) {
        newFurniture = new Chair;
    }
    else if (type == FurnitureType::Table) {
        newFurniture = new Table;
    }
    else if (type == FurnitureType::Sofa) {
        newFurniture = new Sofa;
    }
    return newFurniture;
}


class FurnitureColorDecorator : public IteratorDecorator<class Furniture*>
{
private:
    FurnitureColor TargetColor;

public:
    FurnitureColorDecorator(Iterator<Furniture*> *it, FurnitureColor color)
    : IteratorDecorator<Furniture*>(it), TargetColor(color) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor);
    }
};

class FurnitureExpenDecorator : public IteratorDecorator<class Furniture*>
{
private:
    bool TargetExpen;

public:
    FurnitureExpenDecorator(Iterator<Furniture*> *it, bool isExpen)
    : IteratorDecorator<Furniture*>(it), TargetExpen(isExpen) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsExpen() != TargetExpen)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsExpen() != TargetExpen);
    }
};

void cleanAll(Iterator<Furniture*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Furniture *currentFurniture = it->GetCurrent();
        currentFurniture->clean();
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    size_t N = 24;

    // 3. Массив мебели
    ArrayClass<Furniture*> furnitureArray;
    for(size_t i=0; i<10; i++)
    {
        int furniture_num = rand()%3+1;
        FurnitureType furniture_type = static_cast<FurnitureType>(furniture_num);
        Furniture *newFurniture = CreateFurniture(furniture_type);
        furnitureArray.Add(newFurniture);
    }

    cout << "Array of furniture: " << furnitureArray.Size() << endl;

    // Связанный список мебели

    list<Furniture*> furnitureVector;
    for(size_t i=0; i<N; i++)
    {
        int furniture_num = rand()%3+1;
        FurnitureType furniture_type = static_cast<FurnitureType>(furniture_num);
        Furniture *newFurniture = CreateFurniture(furniture_type);
        furnitureVector.push_back(newFurniture);
    }

    cout << "Size list: " << furnitureVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Cleaning all in a simple loop:" << endl;
    for(size_t i=0; i<furnitureArray.Size(); i++)
    {
        Furniture *currentFurniture = furnitureArray[i];
        currentFurniture->assemble();
    }

    cout << endl << "Cleaning all expensive using iterator:" << endl;
    Iterator<Furniture*> *goodIt = new FurnitureExpenDecorator(furnitureArray.GetIterator(), true);
    cleanAll(goodIt);
    delete goodIt;

    // Обход всех cтульев
    cout << endl << "Cleaning all brown using iterator:" << endl;
    Iterator<Furniture*> *brownIt = new FurnitureColorDecorator(furnitureArray.GetIterator(), FurnitureColor::Brown);
    cleanAll(brownIt);
    delete brownIt;

    // Обход всех дорогих коричневых объектов мебели
    cout << endl << "Cleaning all expensive brown using iterator:" << endl;
    Iterator<Furniture*> *expenBrownIt =
        new FurnitureExpenDecorator(new FurnitureColorDecorator(furnitureArray.GetIterator(), FurnitureColor::Brown), true);
    cleanAll(expenBrownIt);
    delete expenBrownIt;

    // Демонстрация работы адаптера
    cout << endl << "Eating all expensive brown using adapted iterator (another container):" << endl;
    Iterator<Furniture*> *adaptedIt = new ConstIteratorAdapter<std::list<Furniture*>, Furniture*>(&furnitureVector);
    Iterator<Furniture*> *adaptedBrownIt = new FurnitureExpenDecorator(new FurnitureColorDecorator(adaptedIt, FurnitureColor::Brown), true);
    cleanAll(adaptedBrownIt);
    delete adaptedBrownIt;

    return 0;
}
