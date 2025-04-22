//
// Created by joshu on 05/03/2025.
//

#ifndef LITERAL_H
#define LITERAL_H

#include <map>
#include <memory>
#include "Node.h"
# include "Context.h"
class Context; // decleration to allow use of context without circular loop

class Literal {
public:
    Literal();
    void setContext(Context* context);
    [[nodiscard]] Context* getContext() const;
    void setPosition(const std::map<std::string, std::string> &pos);
    [[nodiscard]] std::map<std::string, std::string> getPosition() const;

    [[nodiscard]] virtual std::unique_ptr<Literal> add(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> subtract(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> multiply(const Literal &other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> divide(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> modulo(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareTE(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareNE(const Literal& other) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareLT(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareLTE(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareGT(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> compareGTE(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> andWith(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> orWith(const Literal& other) const;
    [[nodiscard]] virtual std::unique_ptr<Literal> notSelf() const;

    [[nodiscard]] virtual double getNumberValue() const = 0;
    [[nodiscard]] virtual bool getBoolValue() const = 0;
    [[nodiscard]] virtual std::string getStringValue() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Literal> clone() const = 0;
    virtual void printLiteral(std::ostream& os, int tabCount) const;
    friend std::ostream& operator<<(std::ostream& os, const Literal &literal);
    virtual ~Literal() = default;
protected:
    std::unique_ptr<Literal> setLiteral(std::unique_ptr<Literal> literal) const;
    std::map<std::string, std::string> position;
    Context* context;
};


class BoolLiteral final : public Literal {
public:
    explicit BoolLiteral(bool value);

    [[nodiscard]] std::unique_ptr<Literal> add(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> subtract(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> multiply(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> divide(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> modulo(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareTE(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareNE(const Literal& other) const override;

    [[nodiscard]] double getNumberValue() const override;
    [[nodiscard]] bool getBoolValue() const override;
    [[nodiscard]] std::string getStringValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    bool value;

};


class StringLiteral final : public Literal {
public:
    explicit StringLiteral(const std::string &value);

    [[nodiscard]] std::unique_ptr<Literal> add(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> subtract(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> multiply(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> divide(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> modulo(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareTE(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareNE(const Literal& other) const override;

    [[nodiscard]] double getNumberValue() const override;
    [[nodiscard]] bool getBoolValue() const override;
    [[nodiscard]] std::string getStringValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    std::string value;
};


class NumberLiteral : public Literal{
public:
    NumberLiteral();

    [[nodiscard]] std::unique_ptr<Literal> add(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> subtract(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> multiply(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> divide(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> modulo(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareTE(const Literal& other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareNE(const Literal& other) const override;

    [[nodiscard]] double getNumberValue() const override = 0;
    [[nodiscard]] bool getBoolValue() const override = 0;
    [[nodiscard]] std::string getStringValue() const override = 0;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override = 0;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    static std::unique_ptr<NumberLiteral> makeResultLiteral(float value);
};


class IntLiteral final : public NumberLiteral{
public:
    explicit IntLiteral(int value);
    [[nodiscard]] double getNumberValue() const override;
    [[nodiscard]] bool getBoolValue() const override;
    [[nodiscard]] std::string getStringValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    int value;
};


class FloatLiteral final : public NumberLiteral{
public:
    explicit FloatLiteral(float value);
    [[nodiscard]] double getNumberValue() const override;
    [[nodiscard]] bool getBoolValue() const override;
    [[nodiscard]] std::string getStringValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    float value;
};


class FunctionLiteral final : public Literal {
public:
    explicit FunctionLiteral(
        std::string name,
        std::vector<Token> args,
        std::vector<std::unique_ptr<Node>> body,
        std::unique_ptr<Context> scope
        );
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const std::vector<Token>& getArgs() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getBody() const;
    [[nodiscard]] std::unique_ptr<Literal> add(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> subtract(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> multiply(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> divide(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> modulo(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareTE(const Literal &other) const override;
    [[nodiscard]] std::unique_ptr<Literal> compareNE(const Literal &other) const override;
    [[nodiscard]] double getNumberValue() const override;
    [[nodiscard]] bool getBoolValue() const override;
    [[nodiscard]] std::string getStringValue() const override;
    [[nodiscard]] std::unique_ptr<Literal> clone() const override;
    void printLiteral(std::ostream &os, int tabCount) const override;
private:
    std::string name;
    std::vector<Token> argTokens;
    std::vector<std::unique_ptr<Node>> bodyNodes;
    std::unique_ptr<Context> scopeContext;
};
#endif //LITERAL_H
