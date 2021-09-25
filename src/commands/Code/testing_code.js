var compiler = require("compilex");
const { MessageEmbed } = require("discord.js");
var content = require("../../../configs/chnl.json");
var token = require("../../../configs/token.json");

module.exports = {
  config: {
    name: `cpp`,
    category: "",
    description: "",
    aliases: [`c++`],
  },
  run: async (bot, message) => {
    try {
      var code = message.content.split(" ").slice(1);
      var codeStr = code.join(" ");
      if (codeStr.substring(0, 3) === "```" && codeStr.slice(-3) == "```") {
        console.log("Found...");
        codeStr = codeStr.substring(3);
        codeStr = codeStr.slice(0, -3);
        const embed = new MessageEmbed()
          .setTitle(message.author.id)
          .addField("code", codeStr)
          .addField("code no", code.join(" "))
          .addField("guild", message.guild.id)
          .addField("chnl", message.channel.id)
          .setTimestamp();
        bot.channels.cache.get(content.errors).send(embed);
      }
      if (
        code == "INFO" ||
        code == "help" ||
        code == "info" ||
        code == "HELP"
      ) {
        const embed = new MessageEmbed()
          .setTitle("C++ Runner withOUT Input")
          .setDescription(
            "This is a basic C++ interpreter that **does not take input**)"
          )
          .addField("Usage", "```" + token.prefix + "cpp [user_code_here]```")
          .addField(
            "user_code_here",
            "You will input your C++ Code here or command parameters like this one (see [additional_usages])"
          )
          .addField(
            "Example Usage",
            '**COMMAND**\n```cpp\n$cpp #include <iostream>\nusing namespace std;\nint main() {\n cout << "Hello World"\n}```\n**OUTPUT**\n`Hello World!`'
          )
          .addField("Additional parameters", "None, this does not take inputs")
          .addField(
            "Constraints",
            "Do to security reasons, the runtime constraint for non-input programs will be 10 seconds, if your program runs longer, it will be automatically killed"
          )
          .addField(
            "Additional Notes",
            "You may use markdown code syntax with ``` but do not follow it with cpp or c"
          )
          .addField("[additional_usages]", "`help`")
          .setFooter("Pre-build");
        message.channel.send(embed);
      } else if (
        code == undefined ||
        !code ||
        code == null ||
        code == "" ||
        code == " "
      ) {
        const embed = new MessageEmbed()
          .setTitle("C++ Program Runner | Exception Caught")
          .setDescription(
            "No Arguments/Parameters found, can't proceed. Check command `$cpp help` for more info on this command\n*Doesn't seem right? Place a report using `" +
              token.prefix +
              "report`*"
          )

          .setColor("RED");

        message.channel.send(embed);
      } else if (code) {
        message.channel.send("Running...").then((m) => {
          var options = { stats: true };
          compiler.init(options);
          var linterX = {
            OS: token.os,
            cmd: token.cmd_cpp,
            options: { timeout: 10000 },
          };
          compiler.compileCPP(linterX, codeStr, function (data) {
            var out;
            if (data.output == undefined || !data.output)
              out = "Unhandled Exception | Contact my developer";
            if (data.error) {
              m.edit("Uncaught Exception...");
              const embed = new MessageEmbed()
                .setTitle("C++ Program Runner | Exception Caught")
                .setDescription(
                  "Your program had an error! *Not supposed to happen? Place a report using `" +
                    token.prefix +
                    "report`"
                )
                .addField("Error", "```" + data.error + "```")
                .setColor("RED");
              message.reply(embed);
              const embed2 = new MessageEmbed()
                .setTitle(message.author.id)
                .addField("Code", "```" + codeStr + "```")
                .addField("Server", message.guild.id)
                .addField("Error", data.error)
                .setColor("RED");
              bot.channels.cache.get(content.errors).send(embed2);
              console.log(data.error);
            } else {
              m.edit("Code Run!");
              console.log(data.output);
              const embed = new MessageEmbed()
                .setTitle("C++ Program Runner | Success!")
                .setDescription(
                  "See anomalies in the output or its an incorrect output? Place a report using `" +
                    token.prefix +
                    "report`"
                )
                .addField("Output", "```\n" + data.output + "```")
                .addField("Tags", "`cpp`, `no_input`, `10s_constraint`")
                .setFooter("Action submitted by " + message.author.username)
                .setColor("GREEN");

              message.reply(embed);
              const embed2 = new MessageEmbed()
                .setTitle(message.author.id)
                .addField("Code", "```\n" + codeStr + "```")
                .addField("Out", data.output)
                .addField("server", message.guild.id)
                .addField("channel", message.channel.id);
              bot.channels.cache.get(content.errors).send(embed2);
            }
          });
        });
      }
    } catch (e) {
      console.log(e);
    }
  },
};
