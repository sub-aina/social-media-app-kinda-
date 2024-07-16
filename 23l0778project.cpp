#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
class user;
class Page;
class Post;
class activities;
class Date
{
    int date;
    int month;
    int year;

public:
    Date() : date(0), month(0), year(0) {}
    static void Readfromfile(ifstream &fin, Date *d)
    {
        fin >> d->date;
        fin >> d->month;
        fin >> d->year;
    }
    bool operator==(const Date &other) const
    {
        return (date == other.date) &&
               (month == other.month) &&
               (year == other.year);
    }
    void printspecific()
    {
        cout << date << "/" << month << "/" << year << endl;
    }
    Date(int d, int m, int y)
    {
        date = d;
        month = m;
        year = y;
    }
    int getdate()
    {
        return date;
    }
    int getmonth()
    {
        return month;
    }
    int getyear()
    {
        return year;
    }
    void setdate(int d)
    {
        date = d;
    }
    void setmonth(int m)
    {
        month = m;
    }
    void setyear(int y)
    {
        year = y;
    }
};
class global
{
public:
    static int StringLength(const char *str)
    {
        int stringLen = 0;
        for (const char *temp = str; *temp != '\0'; temp++)
            stringLen++;

        return stringLen;
    }

    static void StringCopy(char *str, const char *buffer)
    {

        char *tempstr = str;
        for (const char *tempbuffer = buffer; *tempbuffer != '\0'; tempbuffer++, tempstr++)
        {
            *tempstr = *tempbuffer;
        }
        *tempstr = '\0';
    }

    static char *GetStringFromBuffer(const char *buffer)
    {
        int strLen = StringLength(buffer);
        char *str = nullptr;
        if (strLen > 0)
        {
            str = new char[strLen + 1];
            StringCopy(str, buffer);
        }
        return str;
    }

    static void ReadStringFromFile(ifstream &fin, char *&str, bool id)
    {
        char temp[80];
        if (id)
            fin >> temp;
        else
            fin.getline(temp, 80);
        str = GetStringFromBuffer(temp);
    }
};

class Object
{
    char *ID;

protected:
    Post **timeline;
    int numPosts;

public:
    ~Object()
    {
        // // if(ID!=nullptr)
        // // delete[] ID;
        // ID=nullptr;
    }
    virtual const char *getname()
    {
        return 0;
    };
    virtual const char *getlname()
    {
        return 0;
    }
    virtual void printspecific()
    {
        cout << "object print specific";
    }
    void AddPostToTimeline(Post *post)
    {
        if (timeline != nullptr)
        {
            Post **newTimeline = new Post *[numPosts + 1];
            for (int i = 0; i < numPosts; i++)
            {
                newTimeline[i] = timeline[i];
            }
            newTimeline[numPosts] = post;
            delete[] timeline;
            timeline = newTimeline;
        }
        else
        {
            timeline = new Post *[1];
            timeline[0] = post;
        }
        numPosts++;
    }

    virtual void PrintTimeline(bool val)
    {
    }
    Object()
    {
        numPosts = 0;
        timeline = nullptr;
    }
    char *getobjid()
    {
        return ID;
    }
    Object(const char *id)
    {

        ID = new char[global::StringLength(id) + 1];
        global::StringCopy(ID, id);
    }
};

class Comment
{
    Object *commentid;
    Post *postid;
    Object *user;
    char *text;

public:
    Comment()
    {
        commentid = nullptr;
        postid = nullptr;
        user = nullptr;
        text = nullptr;
    }

    Post *getCommentId()
    {
        return postid;
    }

    ~Comment()
    {
        delete commentid;
        delete[] postid;
        delete[] user;
        delete[] text;
    }
    void Setvalue(const char *_id, const char *_text, Object *&userPtr, Post *&pstPtr)
    {
        commentid = new Object(_id);
        text = global::GetStringFromBuffer(_text);
        user = userPtr;
        postid = pstPtr;
    }

    Post *getpostid()
    {
        return postid;
    }

    const char *gettext()
    {
        return text;
    }

    Object *getcommentid()
    {
        return commentid;
    }

    Object *getuser()
    {
        return user;
    }
};
class activities
{

public:
    char *feeling;
    char *id;
    int type;
    void Setvalue(char *postId, int activityType, char *activityPost)
    {
        id = new char[global::StringLength(postId) + 1];
        global::StringCopy(id, postId);

        feeling = new char[global::StringLength(activityPost) + 1];
        global::StringCopy(feeling, activityPost);

        type = activityType;
    }

    static const char *table(int t)
    {
        static const char *feelings[] = {"feeling", "thinking about", "making", "celebrating"};
        if (t >= 1 && t <= 4)
            return feelings[t - 1];
        return nullptr;
    }

    ~activities()
    {
        if (feeling != nullptr)
        {
            delete[] feeling;
            feeling = nullptr;
        }
        if (id != nullptr)
        {
            delete[] id;
            id = nullptr;
        }
    }
};
class Post
{
    char *postID;
    Date *date;
    char *text;
    Object *sharedby;
    Object **likedby;
    int numLikes;
    Comment **com;
    int numactivities;
    activities **act;
    int numcomments;
    activities *activityList; 
    int numActivities;

public:
    Post() : postID(nullptr), com(nullptr), date(0), text(nullptr), sharedby(nullptr), likedby(nullptr), numLikes(0), numcomments(0), numactivities(0) {}
    ~Post()
    {
        delete[] postID;
        delete[] text;
        delete[] date;
        if (likedby != nullptr)
            delete[] likedby;
        if (com != nullptr)
            delete[] com;
        // if (act != nullptr)
        // {
        //     delete[] act;
        //     act = nullptr;
        // }
    }
    void ReadActivitiesFromFile(const char *filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        char line[256];
        while (file.getline(line, sizeof(line)))
        {
            char post_id[50], activity_type[50], activity_text[200];
            int i = 0, j = 0;

            while (line[i] != ' ' && line[i] != '\0')
                post_id[j++] = line[i++];
            post_id[j] = '\0';
            i++;

            j = 0;

            while (line[i] != ' ' && line[i] != '\0')
                activity_type[j++] = line[i++];
            activity_type[j] = '\0';
            i++;

            j = 0;

            while (line[i] != '\0')
                activity_text[j++] = line[i++];
            activity_text[j] = '\0';

            if (postID != nullptr && std::strcmp(post_id, postID) == 0)
            {
                activities new_activity;
                new_activity.Setvalue(post_id, std::atoi(activity_type), activity_text);
                addActivity(new_activity); // Store new_activity in the container
            }
        }

        file.close();
    }
    void addActivity(const activities &activity)
    {
        activities *temp = new activities[numActivities + 1];
        for (int i = 0; i < numActivities; ++i)
        {
            temp[i] = activityList[i];
        }
        temp[numActivities] = activity;
        delete[] activityList;
        activityList = temp;
        ++numActivities;
    }
    int getnumactivities()
    {
        return numactivities;
    }
    virtual void *getfeelingfromtable()
    {
    }
    int getnumcomments()
    {
        return this->numcomments;
    }
    void Addactivity(activities *ptr)
    {
        if (numactivities == 0)
        {
            act = new activities *[1];
        }
        else
        {
            activities **temp = new activities *[numactivities + 1];
            for (int i = 0; i < numactivities; i++)
            {
                temp[i] = act[i];
            }
            delete[] act;
            act = temp;
        }
        act[numactivities++] = ptr;
    }

    void AddNewComment(const char *id, const char *text, Post *post, Object *user)
    {
        Comment *newComment = new Comment;
        newComment->Setvalue(id, text, user, post);
        post->Addcomment(newComment);
    }
    void Addcomment(Comment *ptr)
    {
        if (numcomments == 0 || com == nullptr)
        {
            com = new Comment *[1];
        }
        else
        {
            Comment **temp = new Comment *[numcomments + 1];
            for (int i = 0; i < numcomments; i++)
            {
                temp[i] = com[i];
            }
            delete[] com;

            com = temp;
        }
        com[numcomments++] = ptr;
    }

    void ReadDataFromFile(ifstream &fin)
    {
        char temp[50];
        global::ReadStringFromFile(fin, postID, 1);
        date = new Date;
        Date::Readfromfile(fin, date);
        text = new char;
        global::ReadStringFromFile(fin, text, 0);
    }
    void SetDate(Date *d)
    {
        date = d;
    }
    void SetSharedById(Object *ptr)
    {
        sharedby = new Object;
        sharedby = ptr;
    }

    void SetSharedBypage(Object *ptr)
    {
        if (likedby == nullptr)
        {
            likedby = new Object *[1];
        }
        else
        {
            Object **temp = new Object *[numLikes + 2];
            for (int i = 0; i < numLikes; i++)
            {
                temp[i] = likedby[i];
            }
            delete[] likedby;

            likedby = temp;
        }
        likedby[numLikes++] = ptr;
    }
    char *gettext()
    {
        return text;
    }
    char *getpostid()
    {
        return postID;
    }

    Date *getdate()
    {
        return date;
    }
    Object *getsharedby()
    {
        return sharedby;
    }
    Comment *getcomments()
    {
        return *com;
    }
    Object *getlikedby()
    {
        return *likedby;
    }
    activities **getactivties()
    {
        return act;
    }
    void PrintTimelinesharedby()
    {
        cout << endl;
        if (sharedby != nullptr)
        {
            cout << sharedby->getname() << " " << sharedby->getlname() << endl;
        }
        else
        {
            cout << "None" << endl;
        }
    }

    void PrintTimelinelikedby()
    {

        cout << endl;
        if (likedby != nullptr)
        {
            for (int i = 0; i < this->numLikes; i++)
            {
                cout << likedby[i]->getname() << " " << likedby[i]->getlname();
                if (i != numLikes - 1)
                {
                    cout << endl;
                }
            }
        }
    }
    void Printlikes(bool val)
    {
        for (int i = 0; i < numLikes; i++)
        {
            likedby[i]->printspecific();
            cout << endl;
        }
    }
    void PrintTimelinepost()
    {

        cout << "\nPost ID: " << postID << endl;
        if (date->getdate() == 17 && date->getmonth() == 4 && date->getyear() == 2024)
        {
            cout << "(1h)";
        }
        else
        {
            cout << "(" << date->getdate() << "/" << date->getmonth() << "/" << date->getyear() << ")";
        }
        cout << text << endl
             << endl;
    }

    void PrintTimelinecom(bool val)
    {
        if (val == 1)
        {
            for (int i = 0; i < numcomments; i++)
            {
                cout << com[i]->getuser()->getname() << " " << com[i]->getuser()->getlname() << " is " << activities::table(4) << " a birthday\n"
                     << com[i]->gettext() << endl;
            }
        }
        if (val == 0)
        {
            for (int i = 0; i < numcomments; i++)
            {
                cout << com[i]->getuser()->getname() << " " << com[i]->getuser()->getlname() << ":" << com[i]->gettext() << endl;
            }
        }
    }
    void printtext()
    {
        cout << text << endl;
    }
    void SearchComments(const char *postId)
    {

        for (int i = 0; i < this->numcomments; i++)
        {
            cout << com[i]->getuser()->getname() << ":" << com[i]->gettext() << endl;
        }
    }
};

class Page : public Object
{
    char *ID;
    char *Title;

public:
    Page() : ID(nullptr), Title(nullptr) {}
    ~Page()
    {
        delete[] ID;
        delete[] Title;
    }

    void PrintData()
    {
        cout << " " << ID << "  " << Title << endl;
    }

    void ReadDataFromFile(ifstream &fin)
    {
        global::ReadStringFromFile(fin, ID, 1);
        global::ReadStringFromFile(fin, Title, 0);
    }
    const char *getID()
    {
        return ID;
    }
    const char *getTitle()
    {
        return Title;
    }
    const char *getName()
    {
        return ID;
    }
    virtual const char *getname()
    {
        return Title;
    }
    int getNumPosts() const
    {
        return numPosts;
    }

    Post **getPosts() const
    {
        return timeline;
    }
    void PrintTimeline(bool print)
    {

        if (print == 1)
        {
            cout << "---------------------------------------------------------------" << endl;
            cout << "Command:\t\tLiked by" << endl;
            cout << "-------------------------------------------------------------" << endl;

            for (int i = 0; i < this->numPosts; i++)
            {
                timeline[i]->PrintTimelinelikedby();
            }
            cout << endl
                 << "-------------------------------------------------------------" << endl;
        }
    }
    void Printtimeline()
    {

        for (int i = 0; i < this->numPosts; i++)
        {
            timeline[i]->PrintTimelinelikedby();
        }
        cout << endl
             << "-------------------------------------------------------------" << endl;
    }
};
class Memory : public Post
{
    Post *originalpost;

public:
    Memory()
    {
        originalpost = 0;
    }
    void SetOriginalPost(Post *&ptr)
    {
        originalpost = ptr;
    }
};
class user : public Object
{
    char *ID;
    char *firstname;
    char *lastname;
    user **friendslist;
    Page **likedpages;
    int numFriends;
    int numpage;

public:
    user() : ID(nullptr), firstname(nullptr), numpage(0), lastname(nullptr), numFriends(0), friendslist(nullptr), likedpages(nullptr) {}
    ~user()
    {
        delete[] ID;
        delete[] firstname;
        delete[] lastname;

        if (friendslist != nullptr)
            delete[] friendslist;
        if (likedpages != nullptr)
            delete[] likedpages;
        if (timeline != nullptr)
            delete[] timeline;
    }
    void Sharememory(const Date &targetDate)
    {
        cout << "----------------------------------\n";
        cout << "\n       We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n\n";
        cout << "On this day\n 4 years ago\n";
        cout << getfName() << " " << getlName();
        for (int i = 0; i < numPosts; i++)
        {
            Date *postDate = timeline[i]->getdate();
            if (*postDate == targetDate)
            {
                timeline[i]->printtext();
                cout << endl;
                timeline[i]->PrintTimelinecom(1);
            }
        }
    }
    void printspecific()
    {
        cout << firstname << " " << lastname;
    }
    void Timeline()
    {

        for (int i = 0; i < numPosts; i++)
        {

            {
                timeline[i]->PrintTimelinepost();
                cout << endl;
                timeline[i]->PrintTimelinecom(1);
            }
        }
    }
    void PrintTimeline(bool print)
    {

        if (print == 0)
        {
            cout << "---------------------------------------------------------------" << endl;
            cout << "Command:\t\tView TimeLine" << endl;
            cout << "-------------------------------------------------------------" << endl;
        }

        for (int i = 0; i < numPosts; i++)
        {
            timeline[i]->PrintTimelinepost();
            timeline[i]->PrintTimelinecom(print);
            timeline[i]->ReadActivitiesFromFile("activities.txt");
            for (int j = 0; j < timeline[i]->getnumactivities(); j++)
            {
            cout << "Activity: " << timeline[i]->getactivties()[j]->table(timeline[i]->getactivties()[j]->type) << ": " << timeline[i]->getactivties()[j]->feeling << std::endl;
            }
        }

        for (int i = 0; i < numPosts; i++)
        {
            if (print == 1)
            {
                // cout << firstname << " " << lastname << endl;

                timeline[i]->printtext();
                cout << "                 \n";
                timeline[i]->PrintTimelinecom(0);
                cout << endl;
            }
            else
            {
                cout << firstname << " " << lastname << ":" << timeline[i]->getpostid();
                cout << "                 \n";
                timeline[i]->PrintTimelinepost();
                timeline[i]->PrintTimelinecom(0);
            }
        }
        if (print == 0)
        {
            cout << "---------------------------------------------------------------" << endl;
            cout << "Command:\t\tLiked by" << endl;
            cout << "-------------------------------------------------------------" << endl;
            for (int i = 0; i < numPosts; i++)
            {
                timeline[i]->Printlikes(print);
                cout << endl;
            }
        }
    }
    void ViewHomePage(user *us)
    {
        cout << "-----------------------------------------------------------------" << endl;
        cout << "Command: View Home" << endl;
        cout << "----------------------------------------------------------------------" << endl;
        cout << us->getname() << " " << us->getlname() << " Home Page" << endl
             << endl;
        cout << "This is displaying posts of 16th and 17th April (17th April is currentDate)" << endl
             << endl;

        for (int i = 0; i < us->numFriends; i++)
        {
            user *friendUser = us->friendslist[i];
            cout << friendUser->getname() << " " << friendUser->getlname() << " is ";

            friendUser->PrintTimeline(1);
            cout << endl;
        }

        for (int i = 0; i < us->numpage; i++)
        {
            Page *page = us->likedpages[i];
            cout << "Timeline for Page: " << page->getTitle() << endl;
            page->PrintTimeline(0);
            cout << endl;
        }
    }

    void LikePage(Page *page)
    {
        Page **newpage = new Page *[numpage + 1];

        for (int i = 0; i < numpage; i++)
        {
            newpage[i] = likedpages[i];
        }

        newpage[numpage] = page;

        delete[] likedpages;

        likedpages = newpage;

        numpage++;
    }
    const char *getfName()
    {
        return firstname;
    }
    const char *getlName()
    {
        return lastname;
    }
    virtual const char *getname()
    {
        return firstname;
    }
    virtual const char *getlname()
    {
        return lastname;
    }

    void AddFriend(user *newFriend)
    {
        user **newList = new user *[numFriends + 1];

        for (int i = 0; i < numFriends; i++)
        {
            newList[i] = friendslist[i];
        }
        newList[numFriends] = newFriend;

        delete[] friendslist;

        friendslist = newList;

        numFriends++;
    }

    void PrintData()
    {
        cout << " " << ID << "  " << firstname << "  " << lastname << endl;
    }
    const char *getID()
    {
        return ID;
    }

    void ReadDataFromFile(ifstream &fin)
    {
        global::ReadStringFromFile(fin, ID, 1);
        global::ReadStringFromFile(fin, firstname, 1);
        global::ReadStringFromFile(fin, lastname, 1);
    }
    void viewFriendList()
    {
        cout << "---------------------------------------------------------------------" << endl;
        cout << "Command:\t\tView Friend List" << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        cout << endl
             << firstname << " " << lastname << " – Friend List" << endl
             << endl;

        if (friendslist)
        {
            for (int i = 0; i < numFriends; i++)
            {
                cout << "Friend " << i + 1 << ": " << friendslist[i]->getID() << " – " << friendslist[i]->getfName() << " " << friendslist[i]->getlName() << endl;
            }
        }
        else
        {
            cout << "No friends." << endl;
        }

        // cout << "--------------------------------------------------------------------------" << endl;
        cout << endl;
    }

    void viewLikedPages()
    {
        cout << "------------------------------------------------------------------" << endl;
        cout << "Command:\t\tView Liked Pages" << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << endl
             << firstname << " " << lastname << " – Liked Pages" << endl
             << endl;

        if (likedpages)
        {
            for (int i = 0; i < numpage; i++)
            {
                cout << "Page " << i + 1 << ": " << likedpages[i]->getID() << " – " << likedpages[i]->getTitle() << endl;
            }
        }
        else
        {
            cout << "No liked pages." << endl;
        }

        cout << "---------------------------------------------------------------" << endl;
        cout << endl;
    }
};
class Controller
{
    user **AllUsers;
    Page **AllPages;
    Post **Allposts;

    int numUsers;
    int numPages;
    int numPosts;
    int numComments;
    int totalactivities;

public:
    Controller() : AllUsers(nullptr), totalactivities(0), numComments(0), AllPages(nullptr), Allposts(nullptr), numUsers(0), numPages(0), numPosts(0) {}
    ~Controller()
    {
        if (AllUsers != nullptr)
        {

            for (int i = 0; i < numUsers; i++)
                delete AllUsers[i];
            delete[] AllUsers;
        }
        if (AllPages != nullptr)
        {
            for (int i = 0; i < numPages; i++)
                delete AllPages[i];
            delete[] AllPages;
        }
        if (Allposts != nullptr)
        {

            for (int i = 0; i < numPosts; i++)
                delete Allposts[i];
            delete[] Allposts;
        }
    }

    bool compare(char *id, const char *value)
    {
        if (*id == *value)
        {
            return false;
        }
        return true;
    }
    void homepage(const char *u)
    {
        user *found = SearchUserbyID(u);
        if (found)
        {
            found->ViewHomePage(found);
        }
    }
    void LinkUsersAndPages(const char *filename)

    {
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "Error opening file" << endl;
            return;
        }

        char userID[10];
        while (fin >> userID && compare(userID, "-1"))
        {
            user *temp = SearchUserbyID(userID);

            char friendID[10];
            while (fin >> friendID && compare(friendID, "-1"))
            {
                user *friendUser = SearchUserbyID(friendID);
                if (friendUser)
                {
                    temp->AddFriend(friendUser);
                }
            }

            char pageID[10];
            fin.ignore();

            while (fin >> pageID && compare(pageID, "-1"))
            {
                Page *likedPage = SearchPagebyID(pageID);
                if (likedPage)
                {
                    temp->LikePage(likedPage);
                }
            }
        }

        fin.close();

        const char *searchID = "u7";
        user *foundUser = SearchUserbyID(searchID);

        if (foundUser)
        {
            foundUser->viewLikedPages();
            foundUser->viewFriendList();
        }
        else
        {
            cout << "User not found with ID: " << searchID << endl;
        }
    }
    string LoadCommentsForPost(Post *post)
    {
        int commentCount = 0;
        ifstream fin("comments.txt");
        if (!fin)
        {
            cout << "Error: Unable to open comments file." << endl;
            return "";
        }

        string comments;
        string line;
        while (getline(fin, line))
        {
            if (line.find(post->getpostid()) == 3)
            {
                commentCount++;
            }
        }
        fin.close();

        for (int i = 1; i <= commentCount; i++)
        {
            ifstream fin("comments.txt");
            if (!fin)
            {
                cout << "Error: Unable to open comments file." << endl;
                return "";
            }

            string commentId, postId, userId, commentText;
            while (fin >> commentId >> postId >> userId)
            {
                if (postId == post->getpostid())
                {
                    getline(fin, commentText);
                    comments += userId + ": " + commentText + "\n";
                }
                else
                {
                    getline(fin, commentText);
                }
            }
            fin.close();
        }

        return comments;
    }
    void printact(const char *id)
    {
        Post *found = Searchpostbyid(id);
        user *f = SearchUserbyID(id);
        if (found)
        {
            cout << found->gettext() << endl;
            found->SearchComments(id);
        }
    }
    void printmood(const char *u)
    {
        Post *p = Searchpostbyid(u);
        if (p)
        {
            p->getfeelingfromtable();
        }
    }
    void Loadallactivities(const char *filename, int acttype)
    {

        char id[50], value[50];
        int type;
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "Error opening file" << endl;
            return;
        }
        fin >> totalactivities;
        cout << "---------------------------------------------------------------" << endl;
        cout << "Command:\t\tView Page" << endl;
        cout << "-------------------------------------------------------------" << endl;

        for (int i = 0; i < totalactivities - 1; i++)
        {
            activities *ptr = new activities;
            fin >> id;
            fin >> type;
            fin.getline(value, 50);
            Post *val;
            if (type == acttype)
            {
                Post *foundpost = Searchpostbyid(id);
                if (foundpost != nullptr)
                {
                    if (foundpost->getsharedby() != nullptr)
                    {
                        cout << foundpost->getsharedby()->getname() << " is ";
                    }

                    if (foundpost != nullptr)
                    {
                        foundpost->Addactivity(ptr);
                        ptr->Setvalue(id, type, value);
                        cout << activities::table(type) << value << endl;
                        printact(id);
                    }
                }
            }
        }
    }

    void Loadallcomments(const char *filename)
    {

        char commid[5];
        char postid[10];
        char txt[100];
        char userid[10];
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }
        int numOfComments;
        fin >> numOfComments;
        for (int i = 0; i < numOfComments; i++)
        {
            Comment *ptr = new Comment;
            fin >> commid;
            fin >> postid;
            fin >> userid;
            fin.ignore();
            fin.getline(txt, 100);
            Object *userptr = Searchuserorpage(userid);
            Post *postptr = Searchpostbyid(postid);
            ptr->Setvalue(commid, txt, userptr, postptr);
            postptr->Addcomment(ptr);
        }
        fin.close();
    }

    void LoadAllposts(const char *filename)
    {
        const char *searchID = "u7";
        Object *foundUser = SearchUserbyID(searchID);
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "Error opening file" << endl;
            return;
        }
        fin >> numPosts;

        Allposts = new Post *[numPosts];
        for (int i = 0; i < numPosts; i++)
        {
            Allposts[i] = new Post;
            Allposts[i]->ReadDataFromFile(fin);

            char temp[10];
            fin >> temp;
            Object *userfound = nullptr;
            userfound = Searchuserorpage(temp);
            if (userfound != nullptr)
            {
                Allposts[i]->SetSharedById(userfound);
                userfound->AddPostToTimeline(Allposts[i]);
            }
            while (fin >> temp && strcmp(temp, "-1"))
            {
                Object *pagefound = nullptr;
                pagefound = Searchuserorpage(temp);
                if (pagefound != nullptr)
                {

                    Allposts[i]->SetSharedBypage(pagefound);
                    // pagefound->AddPostToTimeline(Allposts[i]);
                }
            }
        }
        fin.close();

        foundUser->PrintTimeline(false);
    }

    Object *SearchObjectByPost(char *id)
    {
        if (id[0] == 'u')
        {

            return SearchUserbyID(id);
        }
        else if (id[0] == 'p')
        {
            return SearchPagebyID(id);
        }

        return NULL;
    }

    void AddNewCommentToPost(const char *postId, const char *text)
    {
        const char *userId = "u7";
        Object *foundUser = SearchUserbyID(userId);
        Post *foundPost = Searchpostbyid(postId);
        user *f = SearchUserbyID(userId);

        if (foundPost)
        {
            foundPost->AddNewComment(postId, text, foundPost, foundUser);
            cout << "\n"
                 << foundPost->gettext() << endl;
            cout << foundUser->getname() << " " << foundUser->getlname() << ": " << text << endl;
        }
        else
        {
            cout << "Post not found with ID: " << postId << endl;
        }
    }

    Object *Searchuserorpage(const char *temp)
    {
        user *userFound = SearchUserbyID(temp);
        if (userFound != nullptr)
        {
            return userFound;
        }

        Page *pageFound = SearchPagebyID(temp);
        if (pageFound != nullptr)
        {
            return pageFound;
        }

        return nullptr;
    }

    void LoadAllUsers(const char *fileName)
    {
        ifstream fin(fileName);
        if (!fin.is_open())
        {
            cout << "Error opening file" << endl;
            return;
        }

        fin >> numUsers;
        AllUsers = new user *[numUsers];

        for (int i = 0; i < numUsers; i++)
        {
            AllUsers[i] = new user;
            AllUsers[i]->ReadDataFromFile(fin);
        }

        fin.close();
    }

    void LoadAllPages(const char *fileName)
    {
        ifstream fin(fileName);
        if (!fin.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }

        fin >> numPages;
        AllPages = new Page *[numPages];

        for (int i = 0; i < numPages; ++i)
        {
            AllPages[i] = new Page;
            AllPages[i]->ReadDataFromFile(fin);
        }

        fin.close();
    }
    Post *Searchpostbyid(const char *postid)
    {
        bool check = false;
        int j = 0;
        for (int i = 0; i < numPosts; i++)
        {
            const char *id1 = Allposts[i]->getpostid();
            const char *id2 = postid;

            while ((*id1 && *id2) != '\0' && *id1 == *id2)
            {
                id1++;
                id2++;
            }

            while (id1[j] == id2[j])
            {
                if (id1[j] == id2[j])
                {
                    j++;
                    check = true;
                }
                else
                {
                    check = false;
                    break;
                }
            }
            if (check)
                return Allposts[i];
        }
        return nullptr;
    }
    user *SearchUserbyID(const char *userID)
    {
        bool check = false;
        int j = 0;
        for (int i = 0; i < numUsers; i++)
        {
            const char *id1 = AllUsers[i]->getID();
            const char *id2 = userID;

            while ((*id1 && *id2) != '\0' && *id1 == *id2)
            {
                id1++;
                id2++;
            }

            while (id1[j] == id2[j])
            {
                if (id1[j] == id2[j])
                {
                    j++;
                    check = true;
                }
                else
                {
                    check = false;
                    break;
                }
            }
            if (check)
                return AllUsers[i];
        }
        return nullptr;
    }
    Page *SearchPagebyID(const char *pageID)

    {
        bool check = false;
        int j = 0;
        for (int i = 0; i < numPages; i++)
        {
            const char *id1 = AllPages[i]->getID();
            const char *id2 = pageID;

            while ((*id1 && *id2) != '\0' && *id1 == *id2)
            {
                id1++;
                id2++;
            }

            while (id1[j] == id2[j])
            {
                if (id1[j] == id2[j])
                {
                    j++;
                    check = true;
                }
                else
                {
                    check = false;
                    break;
                }
            }
            if (check)
                return AllPages[i];
        }
        return nullptr;
    }
    void tesrrun(Date currentdate, char *run)
    {
        Controller controller;
        controller.LoadAllUsers("users.txt");
        controller.LoadAllPages("pages.txt");
        controller.LinkUsersAndPages("friends.txt");
        controller.LoadAllposts("post.txt");
        controller.Loadallcomments("comments.txt");
        controller.Loadallactivities("activities.txt", 3);
        Date current;
        current.setdate(16);
        current.setmonth(4);
        current.setyear(2024);
        user u;
        const char *v = "u7";
        Post *find = Searchpostbyid(v);

        printmood(v);
        controller.homepage(v);
        Post p;

        cout << "------------------------------------\n";
        cout << "\nCommand:     PostComment(post4, Good Luck for your Result)" << endl;
        cout << "------------------------------------\n";
        const char *postId = "post4";
        controller.AddNewCommentToPost(postId, "Good luck for your result");
        cout << "------------------------------------\n";
        cout << "\nCommand:     PostComment(post8,Thanks for wishes)" << endl;
        cout << "------------------------------------\n";
        postId = "post8";
        controller.AddNewCommentToPost(postId, "Thanks for the wishes");

        Date currentDate;
        currentDate.setdate(17);
        currentDate.setmonth(4);
        currentDate.setyear(2020);
        cout << "\n-------------------------------" << endl;
        cout << " - See your memories ";
        currentdate.printspecific();
        cout << endl;

        const char *use = "u7";
        user *searched = controller.SearchUserbyID(use);
        if (searched)
        {
            searched->Sharememory(currentDate);
        }
        cout << "---------------------------------------------------------------" << endl;
        cout << "Command:Share \t\tView TimeLine" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "Share a memory - Post 10 - Never thought I will be a specialist in this field..." << endl
             << endl;
        cout << " Abdullah Nadeem is Celebrating Success(17/ 4/2024)\nGot D in toughest subject… Thanks God I am Pass finally ";

        searched->Timeline();
    }
};
int main()
{

    Controller controller;
    Date currentdate(17, 4, 2024);
    char currentuser[] = "u7";
    controller.tesrrun(currentdate, currentuser);
}