# Contributing to Gwion

:+1::tada: First off, thanks for taking the time to consider making acontribution! :tada::+1:

The following is a set of guidelines for contributing to Gwion. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

#### Table Of Contents / important links 

[Code of Conduct](CODE_OF_CONDUCT.md)

<!-- [What should I know before I get started?](#what-should-i-know-before-i-get-started)
you should have soundpipe installed

[How Can I Contribute?](#how-can-i-contribute)
  * [Reporting Bugs](#reporting-bugs)
  * [Suggesting Enhancements](#suggesting-enhancements)
  * [Your First Code Contribution](#your-first-code-contribution)
  * [Pull Requests](#pull-requests)
-->

[Styleguides](#styleguides)
  * [Git Commit Messages](#git-commit-messages)

<!--
  * [C Styleguide](#javascript-styleguide)

  * [Yacc Styleguide](#yacc-styleguide)

  * [Bison Styleguide](#bison-styleguide)

  * [lua Styleguide](#lua-styleguide)

  * [Shell Styleguide](#shell-styleguide)

  * [Documentation Styleguide](#documentation-styleguide)

[Additional Notes](#additional-notes)
  * [Issue and Pull Request Labels](#issue-and-pull-request-labels)
up-for_grabs and the like
-->

## How Can I Contribute?

### Reporting Bugs
<!--
This section guides you through submitting a bug report for Atom. Following these guidelines helps maintainers and the community understand your report :pencil:, reproduce the behavior :computer: :computer:, and find related reports :mag_right:.

Before creating bug reports, please check [this list](#before-submitting-a-bug-report) as you might find out that you don't need to create one.

When you are creating a bug report, please [include as many details as possible](#how-do-i-submit-a-good-bug-report).
-->
Just Fill out [the required template](ISSUE_TEMPLATE.md).

> **Note:** If you find a **Closed** issue that seems like it is the same thing that you're experiencing, open a new issue and 
include a [reference](https://help.github.com/articles/autolinked-references-and-urls/#issues-and-pull-requests) to the original issue in the body of your new one.

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature")
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
* Dot not add dot at the end
* Capitalize first letter
* Limit the first line to 72 characters or less
* Reference issues and pull requests liberally after the first line
* When only changing documentation, include `[ci skip]` in the commit description
* Consider starting the commit message with an applicable emoji:
    * :art: `:art:` when improving the format/structure of the code
    * :racehorse: `:racehorse:` when improving performance
    * :non-potable_water: `:non-potable_water:` when plugging memory leaks
    * :memo: `:memo:` when writing docs
    * :penguin: `:penguin:` when fixing OS specific stuff
    * :bug: `:bug:` when fixing a bug
    * :green_heart: `:green_heart:` when fixing the CI build
    * :white_check_mark: `:white_check_mark:` when working on tests
    * :lock: `:lock:` when dealing with security
    * :shirt: `:shirt:` when enforcing [style](#styleguides)
