const { Discord, MessageEmbed} = require("discord.js");
module.exports = {
    config: {
        name: `eval`,
        aliases: [``]
    },
    run: async (bot, message, args) => {
        message.delete();
        if (message.author.id !== '709776016314204283') return message.reply("Credentials Mismatch | Access Denied")
        try {
            var code = args.join(" ");
            if (code === "client.token" || code === "bot.token") return;
            var evaled = eval(code);
      
            if (typeof evaled !== "string")
              evaled = require("util").inspect(evaled);
            
            const embed = new MessageEmbed()
              .setColor(0x1fd66e)
              .addField(":inbox_tray: Through: ", `\`\`\`js\n${code}\`\`\``)
              .addField(":inbox_tray: Throughout: ", `\`\`\`js\n${clean(evaled)}\n\`\`\``)
            message.channel.send({embed})
          } catch (err) {
            const embed = new MessageEmbed()
            .setColor(0x1fd66e)
            .addField(":outbox_tray: In: ", `\`\`\`js\n${code}\`\`\``)
            .addField(":outbox_tray: Out: ", `\`\`\`js\n${clean(err)}\`\`\``)
          message.channel.send({embed})
          }
      
      function clean(text) {
        if (typeof(text) === 'string')
          return text.replace(/`/g, '`' + String.fromCharCode(8203)).replace(/@/g, '@' + String.fromCharCode(8203));
        else
            return text;

        }
    }
}
