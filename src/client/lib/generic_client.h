#ifndef GENERIC_CLIENT_H
#define GENERIC_CLIENT_H

class Client {
  public:
    /**
     * @brief   This function will return the value for the key
     *          if it exists; if not, it will throw an exception
     *          (NonExistentKeyException)
     */
    virtual std::string Get(const std::string& key) = 0;

    /**
     * @brief   Sets the value associated with key to value (overwriting 
     *          previous values if the key already existed)
     */
    virtual void Set(const std::string& key, const std::string& value) = 0;
};

#endif // GENERIC_CLIENT_H