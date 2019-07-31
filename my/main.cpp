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
  std::shared_ptr<Port> in_;
  std::shared_ptr<Port> out_;
};

struct Cell
{
  std::vector<std::shared_ptr<Connection>> inputs_{};
  std::vector<std::shared_ptr<Connection>> outputs_{};
};

struct Port
{
  std::shared_ptr<Cell> cell_{nullptr};
};

struct Connector
{

};

struct AND2
{
  AND2(std::shared_ptr<Connection> out, std::shared_ptr<Connection> in1, std::shared_ptr<Connection> in2)
  {
    cell_ = std::make_shared<Cell>();

    cell_->outputs_.emplace_back(out);

    cell_->inputs_.emplace_back(in1);
    cell_->inputs_.emplace_back(in2);

    initOutputConnectionPorts(cell_->outputs_);
    initInputConnectionPorts(cell_->inputs_);
  }

  private:
  std::shared_ptr<Cell> cell_{};

  void initInputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections)
  {
    for(auto& connection : connections)
    {
      initInputConnectionPorts(connection);
    }
  }

  void initOutputConnectionPorts(std::vector<std::shared_ptr<Connection>> connections)
  {
    for(auto& connection : connections)
    {
      initOutputConnectionPorts(connection);
    }
  }

  void initInputConnectionPorts(std::shared_ptr<Connection> connection)
  {
    connection->out_->cell_ = cell_;
  }

  void initOutputConnectionPorts(std::shared_ptr<Connection> connection)
  {
    connection->in_->cell_ = cell_;
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
