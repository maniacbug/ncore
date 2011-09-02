#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include <vector>
#include <string>
#include <map>

/**
 * Dispatches commands to workers
 */
class Dispatcher
{
public:
  typedef bool (*worker)(const std::vector<std::string>&);
protected:
  /**
   * Map of commands to workers
   */
  std::map<std::string,worker> commandmap;

public:
  /**
   * Executes the worker associated with the first token in @p commands
   *
   * @param commands Tokenized command and parameters
   *
   * @retval true Worker function found and returned true
   * @retval false Worker function not found or returned false
   */
  bool execute(const std::vector<std::string>& commands) const;
  bool execute(const std::string& commands) const;

  /**
   * Adds a worker
   *
   * @param command Single token for the command
   * @param fn Worked function to execute for the command
   *
   * @retval true Worker added successfully
   * @retval false Problema adding worker, not added
   */
  bool add(const std::string& command,worker fn);

  /**
   * Empties the entire map
   */
  void clear(void);

  size_t size(void) const
  {
    return commandmap.size();
  }
};

#endif // __DISPATCHER_H__

// vim:cin:ai:sts=2 sw=2 ft=cpp
