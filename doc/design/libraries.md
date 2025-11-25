# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libquercus_cli*         | RPC client functionality used by *quercus-cli* executable |
| *libquercus_common*      | Home for common functionality shared by different executables and libraries. Similar to *libquercus_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libquercus_consensus*   | Stable, backwards-compatible consensus functionality used by *libquercus_node* and *libquercus_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libquercusconsensus*    | Shared library build of static *libquercus_consensus* library |
| *libquercus_kernel*      | Consensus engine and support library used for validation by *libquercus_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libquercusqt*           | GUI functionality used by *quercus-qt* and *quercus-gui* executables |
| *libquercus_ipc*         | IPC functionality used by *quercus-node*, *quercus-wallet*, *quercus-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libquercus_node*        | P2P and RPC server functionality used by *quercusd* and *quercus-qt* executables. |
| *libquercus_util*        | Home for common functionality shared by different executables and libraries. Similar to *libquercus_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libquercus_wallet*      | Wallet functionality used by *quercusd* and *quercus-wallet* executables. |
| *libquercus_wallet_tool* | Lower-level wallet functionality used by *quercus-wallet* executable. |
| *libquercus_zmq*         | [ZeroMQ](../zmq.md) functionality used by *quercusd* and *quercus-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libquercus_consensus* and *libquercus_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libquercus_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libquercus_node* code lives in `src/node/` in the `node::` namespace
  - *libquercus_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libquercus_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libquercus_util* code lives in `src/util/` in the `util::` namespace
  - *libquercus_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

quercus-cli[quercus-cli]-->libquercus_cli;

quercusd[quercusd]-->libquercus_node;
quercusd[quercusd]-->libquercus_wallet;

quercus-qt[quercus-qt]-->libquercus_node;
quercus-qt[quercus-qt]-->libquercusqt;
quercus-qt[quercus-qt]-->libquercus_wallet;

quercus-wallet[quercus-wallet]-->libquercus_wallet;
quercus-wallet[quercus-wallet]-->libquercus_wallet_tool;

libquercus_cli-->libquercus_util;
libquercus_cli-->libquercus_common;

libquercus_common-->libquercus_consensus;
libquercus_common-->libquercus_util;

libquercus_kernel-->libquercus_consensus;
libquercus_kernel-->libquercus_util;

libquercus_node-->libquercus_consensus;
libquercus_node-->libquercus_kernel;
libquercus_node-->libquercus_common;
libquercus_node-->libquercus_util;

libquercusqt-->libquercus_common;
libquercusqt-->libquercus_util;

libquercus_wallet-->libquercus_common;
libquercus_wallet-->libquercus_util;

libquercus_wallet_tool-->libquercus_wallet;
libquercus_wallet_tool-->libquercus_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class quercus-qt,quercusd,quercus-cli,quercus-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libquercus_wallet* and *libquercus_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libquercus_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libquercus_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libquercus_common* should serve a similar function as *libquercus_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libquercus_util* and *libquercus_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for quercus-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libquercus_kernel* is not supposed to depend on *libquercus_common*, only *libquercus_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libquercus_kernel* should only depend on *libquercus_util* and *libquercus_consensus*.

- The only thing that should depend on *libquercus_kernel* internally should be *libquercus_node*. GUI and wallet libraries *libquercusqt* and *libquercus_wallet* in particular should not depend on *libquercus_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libquercus_consensus*, *libquercus_common*, and *libquercus_util*, instead of *libquercus_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libquercusqt*, *libquercus_node*, *libquercus_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libquercus_node* to *libquercus_kernel* as part of [The libquercuskernel Project #24303](https://github.com/quercus/quercus/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/quercus/quercus/issues/15732)
