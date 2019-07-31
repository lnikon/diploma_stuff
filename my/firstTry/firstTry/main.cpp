#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum CellType
{
    NOT = 0,
    AND,
    NAND,
    OR,
    NOR,
    USER_DEFINED,
    UNKOWN
};

struct Connection;
struct Port;
struct Cell;

struct Connection
{
    Connection()
    {
        in_ = std::make_shared<Port>();
        out_ = std::make_shared<Port>();
    }

    std::shared_ptr<Port> in_{nullptr};
    std::shared_ptr<Port> out_{nullptr};
};

struct Cell
{
    virtual ~Cell() {}

    std::vector<std::shared_ptr<Connection>> inputs_{nullptr};
    std::vector<std::shared_ptr<Connection>> outputs_{nullptr};

protected:
    virtual void initInputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections) = 0;

    virtual void initOutputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections) = 0;

    virtual void initInputConnectionPorts(std::shared_ptr<Connection> connection) = 0;

    virtual void initOutputConnectionPorts(std::shared_ptr<Connection> connection) = 0;
};

struct Port
{
    std::shared_ptr<Cell> cell_{nullptr};
};

struct Connector
{

};

struct AND2 : public Cell
{
    AND2(std::shared_ptr<Connection> out, std::shared_ptr<Connection> in1, std::shared_ptr<Connection> in2)
    {
        outputs_.emplace_back(out);

        inputs_.emplace_back(in1);
        inputs_.emplace_back(in2);

        initOutputConnectionPorts(outputs_);
        initInputConnectionPorts(inputs_);
    }

    protected:

    void initInputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections) override
    {
        for(auto& connection : connections)
        {
            initInputConnectionPorts(connection);
        }
    }

    void initOutputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections) override
    {
        for(auto& connection : connections)
        {
            initOutputConnectionPorts(connection);
        }
    }

    void initInputConnectionPorts(std::shared_ptr<Connection> connection) override
    {
        connection->out_->cell_ = std::make_shared<AND2>(this);
    }

    void initOutputConnectionPorts(std::shared_ptr<Connection> connection) override
    {
        connection->in_->cell_ = std::make_shared<AND2>(this);
    }
};

int main()
{
    std::shared_ptr<Connection> out = std::make_shared<Connection>();
    std::shared_ptr<Connection> in1 = std::make_shared<Connection>();
    std::shared_ptr<Connection> in2 = std::make_shared<Connection>();

    AND2 and2gate(out, in1, in2);

    return 0;
}
