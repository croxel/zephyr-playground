This sub-page will cover the scope of the Jira ticket: [![](https://croxel.atlassian.net/rest/api/2/universal_avatar/view/type/issuetype/avatar/10315?size=medium)CXRD-32](https://croxel.atlassian.net/browse/CXRD-32) - SCM - Define commit structure requirements Done

Commit Messages
---------------

Each commit should follow the next convention:

```java
[JIRA-ID] Commit Title

Commit Body (description of what was done).

Commit signature (git commit -s adds it)
```

*   Example:
    
    ```java
    [CXRD-1] First commit
    
    Adding repository foundations: README and main directories.
    
    Signed-off-by: Luis Ubieda <luisf@croxel.com>
    ```
    
*   Regular expression matching this: `[CXRD-[0-9]+] .+`
    

The details of the gitlint rules applied is captured here: [https://github.com/zephyrproject-rtos/zephyr/blob/v3.6.0/.gitlint](https://github.com/zephyrproject-rtos/zephyr/blob/v3.6.0/.gitlint)

Branching Schema
----------------

*   Main branch is called, by default, `main`
    
*   The current in-progress work will be merged into the `develop` branch (created from `main` one).
    
*   Each particular task which involves repository changes will be created/added in a separated branch (created from `develop` following the below section _Naming Convention_). Once this is done, the corresponding Pull Request will be created to be merged into develop branch.
    
*   When the PR is done/closed, the corresponding branch should be deleted.
    
*   Once the `develop` branch has a considerable amount of functional changes (deliverable version), all this code will be merged into the `main` branch with a particular product version (`major.minor.revision`)
    
*   Ensure **everything we push into main** pass every single workflow, its being deployed into the board and it’s working as expected with certain amount of functional tests.
    

### Naming Convention

Each branch must have the <ticket ID> associated with the JIRA ID + a brief scope of the task separated with “**\_”**. It must be a one on one relationship between JIRA and github branches.

The following words can be added as part of the branch name to give a little more context:

*   <jira\_ID>\_brief\_scope
    
*   doc/<jira\_ID>\_brief\_scope
    
*   resources/<jira\_ID>\_brief\_scope
    
*   feature/<jira\_ID>\_brief\_scope
    
*   bug/<jira\_ID>\_brief\_scope