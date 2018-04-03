# QDbInteractionThread
constrcut :
QThread thread2;
QDbInteractionThread db_object(&thread2);
db_object.query(
    [/*...*/]//thread2 side
    {
        std::vector<int> retVal;
        //...
        return retVal;
    },
    [/*...*/](auto val)//or std::vector<int>, main thread side
    {
    //...
    });
    
db_object.query(
    [/*...*/]//thread2 side
    {
        std::vector<int> retVal;
        //...
        return retVal;
    },
    [/*...*/](auto val)//or std::vector<int>, main thread side
    {
    //...
    retVal std::list<std::string>();
    }).
then_query(
    [](auto val)//std::list<std::string>, thread2 side
    {
        //...
        //no return
    },
    []//main thread side
    {
    //no params for example
    });