//
// Created by joshu on 05/03/2025.
//

#ifndef LITERAL_H
#define LITERAL_H
#include <map>
#include <memory>

class Context; // decleration to allow use of context without circular loop

class Literal {
public:
    Literal();
    void setContext(Context* context);
    [[nodiscard]] Context* getContext() const;
    void setPosition(const std::map<std::string, std::string> &pos);
    [[nodiscard]] virtual std::map<std::string, std::string> getPosition();
    [[nodiscard]] virtual std::unique_ptr<Literal> add(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> subtract(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> multiply(const Literal &other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> divide(const Literal& other) const = 0;
    [[nodiscard]] virtual double getValue() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> clone() const = 0;
    virtual void printLiteral(std::ostream& os, int tabCount) const;
    friend std::ostream& operator<<(std::ostream& os, const Literal &literal);
    virtual ~Literal() = default;
protected:
    std::map<std::string, std::string> position;
    Context* context;
};

class NumberLiteral : public Literal{
public:
    NumberLiteral();
    [[nodiscard]] std::unique_ptr<Literal> add(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> subtract(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> multiply(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> divide(const Literal& other) const override;
    [[nodiscard]] double getValue() const override = 0;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    static std::unique_ptr<NumberLiteral> makeResultLiteral(float value);
};

class IntLiteral final : public NumberLiteral{
public:
    explicit IntLiteral(int value);
    [[nodiscard]] double getValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    int value;
};

class FloatLiteral final : public NumberLiteral{
public:
    explicit FloatLiteral(float value);
    [[nodiscard]] double getValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    float value;
};


#endif //LITERAL_H
