//set this as a cooldown enforced command;
const talkedRecently = new Set();
const { MessageEmbed, DiscordAPIError } = require("discord.js");
const Discord = require("discord.js");
const config = require("../../../configs/token.json");
const chnl = require("../../../configs/chnl.json");
const resource = require("../../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../../configs/colors.json");
const Database = require("easy-json-database");
const token = require("../../../configs/token.json");
module.exports = {
  config: {
    name: `docs`,
    category: "",
    description: "",
    aliases: [`doc`],
  },
  run: async (bot, message, args) => {
    try {
      const db = new Database("db/registry_user.json");
      if (db.has(message.author.id)) {
        if (
          talkedRecently.has(message.author.id) &&
          message.author.id != config.owner_id
        ) {
          message.channel.send(
            "**Command on Cooldown for 30 seconds.**\n*A higher trust factor is required to gain a quicker cooldown.*"
          );
        } else {
          //function only accepts a value of .md or .pdf or .html etc.
          function askFile(filetype) {
            message.channel
              .send(
                "Fetching documentations for type `" +
                  filetype.replace(".", "").toUpperCase() +
                  "`..."
              )
              .then((m) => {
                setTimeout(function () {
                  m.edit(
                    "**Fetched it successfully!**\nOther formats are: `markdown, text, htm, pdf, html, mhtml, xml, ipynb, tex, texinfo`"
                  );
                  const doc = new Discord.MessageAttachment(
                    "lib/archives/bot_docs/README" + filetype
                  );
                  message.channel.send(doc);
                }, 1000);
              });
          }
          function askCooldown(id) {
            talkedRecently.add(id);
            setTimeout(() => {
              talkedRecently.delete(id);
            }, 30000);
          }
          let file_type = args[0];
          if (!file_type || file_type == undefined || file_type == null) {
            const embed = new MessageEmbed()
              .setTitle(
                "Oops, this command requires some input arguments to proceed!"
              )
              .setDescription(
                "This command requires arguments after the inital command in order to give you the correct result!"
              )
              .addField(
                "Command Usage",
                "```" + token.prefix + "docs [file_type]```"
              )
              .addField(
                "file_type",
                "**Description:** The file type specified to view the documentations\n**Avaliable Parameters:** `markdown, text, htm, pdf, html, mhtml"
              )
              .addField(
                "Command Description",
                "Gets the documentation for this service."
              )
              .addField(
                "Command Example Usage",
                "```" + token.prefix + "docs md```"
              )
              .setThumbnail(resource.aw_snap)
              .setColor(colors.error);
            message.channel.send({ embed });
          } else if (file_type == "md" || file_type == "markdown") {
            askFile(".md");
            askCooldown(message.author.id);
          } else if (file_type == "txt" || file_type == "text") {
            askFile(".txt");
            askCooldown(message.author.id);
          } else if (
            file_type == "html" ||
            file_type == "mhtml" ||
            file_type == "htm"
          ) {
            askFile(".html");
            askCooldown(message.author.id);
          } else if (file_type == "pdf") {
            askFile(".pdf");
            askCooldown(message.author.id);
          } else {
            const embed = new MessageEmbed()
              .setTitle(
                "Oops, this command requires some input arguments to proceed!"
              )
              .setDescription(
                "This command requires arguments after the inital command in order to give you the correct result!"
              )
              .addField(
                "Command Usage",
                "```" + token.prefix + "docs [file_type]```"
              )
              .addField(
                "file_type",
                "**Description:** The file type specified to view the documentations\n**Avaliable Parameters:** `markdown, text, htm, pdf, html, mhtml, xml, ipynb, tex, texinfo`"
              )
              .addField(
                "Command Description",
                "Gets the documentation for this service."
              )
              .addField(
                "Command Example Usage",
                "```" + token.prefix + "docs md```"
              )
              .setThumbnail(resource.aw_snap)
              .setColor(colors.error);
            message.channel.send({ embed });
          }
        }
      } else {
        const embed = new MessageEmbed()
          .setTitle("Command only for registered users...")
          .setThumbnail(resource.aw_snap)
          .setColor(colors.error)
          .setDescription(
            "Use `" +
              config.prefix +
              "register` to register yourself in order to use this command!"
          )
          .setFooter("For more support, join the Discord!");

        message.channel.send({ embed });
      }
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
