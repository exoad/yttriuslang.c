module.exports = {
  config: {
    name: `userdocs`,
    category: "",
    description: "",
    aliases: [`ainfo`],
  },
  run: async (bot, message, args) => {
    try {
      message.channel.send("**Fetching Documetations...**").then((m) => {
        setTimeout(function () {
          m.edit(
            "**Fetched the documentations for:** `Accounts & Trust System`"
          );
          const doc = new Discord.MessageAttachment(
            "lib/archives/account_readmes/README.txt"
          );
          message.channel.send(doc);
        }, 1000);
      });
    } catch (err) {
      const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription(
          "Use `" +
            config.prefix +
            "help` for a list of avaliable commands or use `" +
            config.prefix +
            "support` to join the support server!"
        )
        .setFooter("Still facing issues? Join the support server!");
      message.channel.send({ embed }).then((m) => {
        m.delete({ timeout: 5000 });
      });
    }
  },
};
