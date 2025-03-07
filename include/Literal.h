//
// Created by joshu on 05/03/2025.
//

#ifndef LITERAL_H
#define LITERAL_H

class Context; // decleration to allow use of context without circular loop

class Literal {
public:
    Literal();
    Context* getContext();
    void setContext(Context* context);
    friend std::ostream& operator<<(std::ostream& os, const Literal &literal);
    virtual void printLiteral(std::ostream& os, int tabCount) const;
    virtual ~Literal() = default;
private:
    Context* context;
};

class NumberLiteral : public Literal{
public:
    NumberLiteral();
    NumberLiteral* add(NumberLiteral*);
    NumberLiteral* subtract(NumberLiteral*);
    NumberLiteral* multipy(NumberLiteral*);
    NumberLiteral* divide(NumberLiteral*);
    auto getValue();
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
};

class IntLiteral final : public NumberLiteral{
public:
    explicit IntLiteral(int value);
    [[nodiscard]] int getValue() const;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    int value;
};

class FloatLiteral final : public NumberLiteral{
public:
    explicit FloatLiteral(float value);
    [[nodiscard]] float getValue() const;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    float value;
};


#endif //LITERAL_H
