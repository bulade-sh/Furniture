#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates.h"

using namespace std;

enum class FurnitureColor : int {
    Brown,
    Beige,
    Transparent,
    Unknown
};

// Реализация паттерна "Стратегия"
enum class CleaningEnum : int {
    ShakeOff,
    Wipe,
    Vacuum,
    None
};

class CleaningStrategy {
public:
    virtual ~CleaningStrategy() {}
    virtual void clean() = 0;
};

class ShakeOffStrategy : public CleaningStrategy {
    void clean() override { cout << "Shake off the dust..."; }
};

class WipeStrategy : public CleaningStrategy {
    void clean() override { cout << "Wipe with a cloth..."; }
};

class VacuumStrategy : public CleaningStrategy {
    void clean() override { cout << "Vacuum..."; }
};

// Фабрика стратегий очистки
CleaningStrategy* CreateCleaningStrategy(CleaningEnum method) {
    switch(method) {
        case CleaningEnum::ShakeOff: return new ShakeOffStrategy;
        case CleaningEnum::Wipe: return new WipeStrategy;
        case CleaningEnum::Vacuum: return new VacuumStrategy;
        default: return nullptr;
    }
}

class Furniture
{
private:
    string material;
    FurnitureColor color;
    int weight;

    CleaningStrategy* cleaningMethod; // Стратегия очистки

protected:
    bool FurnitureIsExpen;

    // Проверка, можно ли чистить
    virtual bool CanBeCleaned() { return true; }

    // Подготовка к очистке
    virtual void PrepareForCleaning() { cout << "Preparing furniture... "; }

    // Очистка
    void PerformCleaning()
    {
        if (cleaningMethod != nullptr)
        {
            cleaningMethod->clean();
        }
        else
        {
            cout << "No cleaning method set!";
        }
    }

public:
    Furniture(string mater, FurnitureColor clr, int wgt)
        : material(mater), color(clr), weight(wgt), cleaningMethod(nullptr)
    {
        FurnitureIsExpen = static_cast<bool>(rand() % 2);
    }

    virtual ~Furniture()
    {
        if (cleaningMethod != nullptr) delete cleaningMethod;
    }

    virtual void PrintType() = 0;

    // Шаблонный метод очистки
    void clean()
    {
        if (!CanBeCleaned())
        {
            cout << "This furniture cannot be cleaned!" << endl;
            return;
        }
        PrintType();
        cout << " -> ";
        PrepareForCleaning();
        cout << " -> ";
        PerformCleaning();
        cout << endl;
    }

    void SetCleaningStrategy(CleaningStrategy* strategy)
    {
        if (cleaningMethod != nullptr) delete cleaningMethod;
        cleaningMethod = strategy;
    }

    virtual void assemble() = 0;
    virtual void disassemble() = 0;
    virtual void punch() { cout << "PUNCH furniture" << endl; }

    FurnitureColor GetColor() const { return color; }
    bool IsExpen() const { return FurnitureIsExpen; }
};

class Chair : public Furniture
{
private:
    int numLegs;
    bool hasBack;

protected:
    void PrepareForCleaning() override
    {
        cout << "Checking chair stability... ";
    }

public:
    Chair() : Furniture("Wood", FurnitureColor::Brown, 5), numLegs(4), hasBack(true)
    {
        // Установка стратегии по умолчанию
        SetCleaningStrategy(CreateCleaningStrategy(CleaningEnum::ShakeOff));
    }

    void assemble() override
    {
        cout << "Chair assembly with " << numLegs << " legs."
             << (hasBack ? " Has back" : "") << endl;
    }

    void disassemble() override
    {
        cout << "Chair disassembly" << endl;
    }

    void PrintType() { cout << "Chair"; }
};

class Table : public Furniture
{
private:
    string shape;
    int numSeats;

protected:
    void PrepareForCleaning() override
    {
        cout << "Get rid off dishes... ";
    }

public:
    Table() : Furniture("Glass", FurnitureColor::Transparent, 11), shape("round"), numSeats(5)
    {
        SetCleaningStrategy(CreateCleaningStrategy(CleaningEnum::Wipe));
    }

    void assemble() override
    {
        cout << "Table assembly with " << shape << " shape and "
             << numSeats << " seats." << endl;
    }

    void disassemble() override
    {
        cout << "Table disassembly" << endl;
    }

    void PrintType() { cout << "Table"; }
};

class Sofa : public Furniture
{
private:
    int numSeats;
    bool canConvertToBed;

protected:
    void PrepareForCleaning() override
    {
        cout << "Remove clothes and pillows... ";
    }
public:
    Sofa() : Furniture("Fabric", FurnitureColor::Beige, 25), numSeats(4), canConvertToBed(true)
    {
        SetCleaningStrategy(CreateCleaningStrategy(CleaningEnum::Vacuum));
    }

    void assemble() override
    {
        cout << "Sofa assembly with " << numSeats << " seats."
             << (canConvertToBed ? " Can convert to bed." : "") << endl;
    }

    void disassemble() override
    {
        cout << "Sofa disassembly" << endl;
    }

    void PrintType() { cout << "Sofa"; }
};


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

    ArrayClass<Furniture*> furnitureArray;
    for(size_t i=0; i<10; i++)
    {
        int furniture_num = rand()%3+1;
        FurnitureType furniture_type = static_cast<FurnitureType>(furniture_num);
        Furniture *newFurniture = CreateFurniture(furniture_type);

        //newFurniture->SetCleaningStrategy(CreateCleaningStrategy(CleaningEnum::Wipe));

        furnitureArray.Add(newFurniture);
    }

    //cout << "Array of furniture: " << furnitureArray.Size() << endl;


    list<Furniture*> furnitureVector;
    for(size_t i=0; i<N; i++)
    {
        int furniture_num = rand()%3+1;
        FurnitureType furniture_type = static_cast<FurnitureType>(furniture_num);
        Furniture *newFurniture = CreateFurniture(furniture_type);
        furnitureVector.push_back(newFurniture);
    }

    //cout << "Size list: " << furnitureVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Cleaning all in a simple loop:" << endl;
    for(size_t i=0; i<furnitureArray.Size(); i++)
    {
        Furniture *currentFurniture = furnitureArray[i];
        currentFurniture->clean();
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
    cout << endl << "Cleaning all expensive brown using adapted iterator:" << endl;
    Iterator<Furniture*> *adaptedIt = new ConstIteratorAdapter<std::list<Furniture*>, Furniture*>(&furnitureVector);
    Iterator<Furniture*> *adaptedBrownIt = new FurnitureExpenDecorator(new FurnitureColorDecorator(adaptedIt, FurnitureColor::Brown), true);
    cleanAll(adaptedBrownIt);
    delete adaptedBrownIt;

    return 0;
}
