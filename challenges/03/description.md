# Paths

Everything in Linux is a file, as you journey deeper into Linux you’ll understand this, but for now just keep that in mind. 

Every file is organized in a hierarchical directory tree. The first directory in the filesystem is aptly named the root directory.

The root directory has many folders and files which you can store more folders and files, etc.

Here is an example of what the directory tree looks like:

```
/
|-- bin
|   |-- file1
|   |-- file2
|-- etc
|   |-- file3
|   `-- directory1
|       |-- file4
|       `-- file5
|-- home
|-- var
```

The location of these files and directories are referred to as paths.

If you had a folder named home with a folder inside of it named guy and another folder in that folder called Movies, that path would look like this: `/home/guy/Movies`, pretty simple huh?


Navigation of the filesystem, much like real life is helpful if you know where you are and where you are going. To see where you are, you can use the `pwd` command, this command means “print working directory” and it just shows you which directory you are in, note the path stems from the root directory.


# Navigation

Remember we’ll need to navigate our way using paths. There are two different ways to specify a path, with *absolute* and *relative* paths:

- _Absolute path_: This is the path from the root directory. he root directory is commonly shown as a slash. Every time your path starts with `/` it means you are starting from the root directory. For example, `/home/guy/Desktop`.

- _Relative path_: This is the path from where you are currently in filesystem. If I was in location `/home/guy/Documents` and wanted to get to a directory inside Documents called taxes, I don’t have to specify the whole path from root like `/home/guy/Documents/taxes`, I can just go to `taxes/` instead.


It can get pretty tiring navigating with absolute and relative paths all the time, luckily there are some shortcuts to help you out.

- `.`  marks the _current directory_.
- `..` markes the _parent directory_ - which is the directory that contains the current directory.
- `~` marks the _home directory_ - which defaults to your "home directory", such as `/home/guy`.
- `-` marks the _previous directory_ - this will take you to the previous directory you have visited.

Move between directories using the `cd` command.


# Your goal

Find the flag! It is located at a file called `flag.txt`


# Useful commands
- pwd
- ls
- cd
- find


!!! note:
    The `flag` command can receive an optional argument stating a challenge name. You can move to challenges you've solved using that argument. For example - `flag challenge01` will ask you for the first flag you've found (in challenge00). The flag for challenge00 is "none".
